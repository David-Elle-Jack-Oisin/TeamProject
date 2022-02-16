#include <assert.h>
#include <string.h>
#include <string>
#include <random>
#include <thread>
#include <cstring>

#ifndef _PLAYER_H
#define _PLAYER_H
    #include "../simulation/Player.cpp"
#endif
#ifndef _PLAYER_RENDERER_H
#define _PLAYER_RENDERER_H
    #include "../simulation/PlayersRenderer.cpp"
#endif


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
		if (!shutDown) {
			std::string packet = std::to_string(position.x) + ":" + std::to_string(position.y);
        	const char *formatPacket = packet.c_str();
        	clientInstance->SendMessageToConnection(connection, formatPacket, (uint32)strlen(formatPacket), k_nSteamNetworkingSend_UnreliableNoDelay, nullptr);
		}
    }
	void startClient(PlayersRenderer *renderer){
		SteamNetworkingIPAddr serverAddress;
		playRenderer = renderer;
		serverAddress.Clear();
		if (!serverAddress.ParseString("127.0.0.1"))
			fprintf(stderr,"NETWORK: Invalid server address '%s'\n", "127.0.0.1");
		serverAddress.m_port = DEFAULT_SERVER_PORT;
		InitialiseConnectionSockets();
		Run(serverAddress);
		GameNetworkingSockets_Kill();
	}
	void turnOff(){
		shutDown = true;
	}
private:
	PlayersRenderer *playRenderer;
    ISteamNetworkingSockets *clientInstance;
    HSteamNetConnection connection;
    bool shutDown = false;
	bool firstPlayer = true;
	bool recievedId = false;
	int id;

	static void InitialiseConnectionSockets(){
        SteamDatagramErrMsg errorMessage;
        GameNetworkingSockets_Init( nullptr, errorMessage );
        globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
    }

	void Run(const SteamNetworkingIPAddr &serverIp){
        clientInstance = SteamNetworkingSockets();
        char serverAddressBuffer[ SteamNetworkingIPAddr::k_cchMaxString ];
        serverIp.ToString(serverAddressBuffer, sizeof(serverAddressBuffer), true);
        fprintf(stderr,"NETWORK: Connecting to server at %s\n", serverAddressBuffer);
		SteamNetworkingConfigValue_t clientConfig;
		clientConfig.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged);
		connection = clientInstance->ConnectByIPAddress(serverIp, 1, &clientConfig);
		if ( connection == k_HSteamNetConnection_Invalid )
			fprintf(stderr,"NETWORK: Failed to create connection\n");
		generateId();
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
                fprintf(stderr,"NETWORK: Error Checking For Messages\n");
            }
            std::string packet;
			packet.assign((const char *)incomingMessage->m_pData, incomingMessage->m_cbSize);
			if(packet.find("id:") != std::string::npos){
					const char *identifer = packet.substr(packet.find(":") + 1).c_str();
					int id = std::stoi(identifer);
					playRenderer->matchPlayerIdToServer(id);
					fprintf(stderr,"NETWORK: Id Recieved (%s)\n",identifer);
			}
			else if (packet.find(":") != std::string::npos)
			{
				if(firstPlayer == true){
					Player *ptrPlayer;
					ptrPlayer = new Player(2);
					playRenderer->addNewPlayer(ptrPlayer);
					firstPlayer = false;
				}
				const char *firstFloatCharArray = packet.substr(0, packet.find(":")).c_str();
				const char *secondFloatCharArray = packet.substr(packet.find(":") + 1).c_str();
				float firstFloat = std::atof(firstFloatCharArray);
				float secondFloat = std::atof(secondFloatCharArray);
				playRenderer->updateSecondPlayer(firstFloat, secondFloat);
			}
			
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
					fprintf(stderr,"NETWORK: Local Problem (%s)\n", connectionInfo->m_info.m_szEndDebug );
				}
				else if (connectionInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally){
					fprintf(stderr,"NETWORK: Lost Connection with host (%s)\n", connectionInfo->m_info.m_szEndDebug );
				}
				else{
					fprintf(stderr,"NETWORK: Host Disconnected (%s)\n", connectionInfo->m_info.m_szEndDebug);
				}

				clientInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false );
				connection = k_HSteamNetConnection_Invalid;
				break;
			}
			case k_ESteamNetworkingConnectionState_Connecting:
				break;
			case k_ESteamNetworkingConnectionState_Connected:
				fprintf(stderr,"NETWORK: Connected to server OK\n");
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
	int generateId(){
		if (!shutDown){
			std::string packet = "i";
			const char *formatPacket = packet.c_str();
			clientInstance->SendMessageToConnection(connection, formatPacket, (uint32)strlen(formatPacket), k_nSteamNetworkingSend_Reliable, nullptr);
		}
	}
};

gameClient *gameClient::gameClientCallBackInstance = nullptr;
