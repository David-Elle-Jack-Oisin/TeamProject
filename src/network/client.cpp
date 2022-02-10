#include <assert.h>
#include <string.h>
#include <string>
#include <random>
#include <thread>


#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#include "raylib.h"
SteamNetworkingMicroseconds globalLogTimeZero;
const uint16 DEFAULT_SERVER_PORT = 27020;
class gameClient
{
public:
    void sendPos(Vector2 position){
        std::string packet = std::to_string(position.x) + ":" + std::to_string(position.y);
        const char *formatPacket = packet.c_str();
        clientInstance->SendMessageToConnection(connection, formatPacket, (uint32)strlen(formatPacket), k_nSteamNetworkingSend_UnreliableNoDelay, nullptr);
    }
	void startClient(){
		SteamNetworkingIPAddr serverAddress; 
		serverAddress.Clear();
		if (!serverAddress.ParseString("127.0.0.1"))
			fprintf(stderr,"Invalid server address '%s'", "127.0.0.1");
		serverAddress.m_port = DEFAULT_SERVER_PORT;
		fprintf(stderr,"%i",DEFAULT_SERVER_PORT);
		InitialiseConnectionSockets();
		Run(serverAddress);
		GameNetworkingSockets_Kill();
	}
private:
    ISteamNetworkingSockets *clientInstance;
    HSteamNetConnection connection;
    bool shutDown = false;

	static void InitialiseConnectionSockets(){
        SteamDatagramErrMsg errorMessage;
        GameNetworkingSockets_Init( nullptr, errorMessage );
        globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
    }

	void Run(const SteamNetworkingIPAddr &serverIp){
        clientInstance = SteamNetworkingSockets();
        char serverAddressBuffer[ SteamNetworkingIPAddr::k_cchMaxString ];
        serverIp.ToString(serverAddressBuffer, sizeof(serverAddressBuffer), true);
        fprintf(stderr,"Connecting to server at %s", serverAddressBuffer);
		SteamNetworkingConfigValue_t clientConfig;
		clientConfig.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged);
		connection = clientInstance->ConnectByIPAddress(serverIp, 1, &clientConfig);
		if ( connection == k_HSteamNetConnection_Invalid )
			fprintf(stderr,"Failed to create connection");
		while (!shutDown){
			pollIncomingMessages();
			pollConnectionStateChanges();
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}
	}
    void pollIncomingMessages(){
        while(!shutDown){
            ISteamNetworkingMessage *incomingMessage = nullptr;
            int numOfMessages = clientInstance->ReceiveMessagesOnConnection(connection, &incomingMessage, 1);
            if(numOfMessages == 0){
                break;
            }
            if(numOfMessages < 0){
                fprintf(stderr,"Error Checking For Messages");
            }
            std::string packet;
			packet.assign((const char *)incomingMessage->m_pData, incomingMessage->m_cbSize);
			const char *formattedPacket = packet.c_str();
            fprintf(stderr, "RECEIEVED: %s\n", formattedPacket );
			incomingMessage->Release();
        }
    }

    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *connectionInfo){
		assert(connectionInfo->m_hConn == connection || connection == k_HSteamNetConnection_Invalid);
		switch (connectionInfo->m_info.m_eState){
			case k_ESteamNetworkingConnectionState_None:
				break;
			case k_ESteamNetworkingConnectionState_ClosedByPeer:
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:{
				shutDown = true;
				if (connectionInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
				{
					fprintf(stderr,"Local Problem (%s)", connectionInfo->m_info.m_szEndDebug );
				}
				else if (connectionInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally){
					fprintf(stderr,"Lost Connection with host (%s)", connectionInfo->m_info.m_szEndDebug );
				}
				else{
					fprintf(stderr,"Host Disconnected (%s)", connectionInfo->m_info.m_szEndDebug);
				}

				clientInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false );
				connection = k_HSteamNetConnection_Invalid;
				break;
			}
			case k_ESteamNetworkingConnectionState_Connecting:
				break;
			case k_ESteamNetworkingConnectionState_Connected:
				fprintf(stderr,"Connected to server OK");
				break;
			default:
				break;
		}
	}

    static gameClient *gameClientCallBackInstance;
    
    static void ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *connectionInfo){
        gameClientCallBackInstance->OnConnectionStatusChanged(connectionInfo);
    }
    void pollConnectionStateChanges(){
        gameClientCallBackInstance = this;
        clientInstance->RunCallbacks();
    }
};

gameClient *gameClient::gameClientCallBackInstance = nullptr;
