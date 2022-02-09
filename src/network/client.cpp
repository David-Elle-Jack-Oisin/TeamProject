#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <cctype>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#ifdef WIN32
	#include <windows.h> // Ug, for NukeProcess -- see below
#else
	#include <unistd.h>
	#include <signal.h>
#endif
SteamNetworkingMicroseconds globalLogTimeZero;
class gameClient
{
public:
    void Run(const SteamNetworkingIPAddr &serverIp){
        clientInstance = SteamNetworkingSockets();
        char serverAddressBuffer[ SteamNetworkingIPAddr::k_cchMaxString ];
        serverIp.ToString(serverAddressBuffer, sizeof(serverAddressBuffer), true);
        printf("Connecting to server at %s", serverAddressBuffer);
		SteamNetworkingConfigValue_t clientConfig;
		clientConfig.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged);
		connection = clientInstance->ConnectByIPAddress(serverIp, 1, &clientConfig);
		if ( connection == k_HSteamNetConnection_Invalid )
			printf("Failed to create connection");
		while (!shutDown){
			pollIncomingMessages();
			pollConnectionStateChanges();
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}
	}
    static void InitialiseConnectionSockets(){
            SteamDatagramErrMsg errorMessage;
            GameNetworkingSockets_Init( nullptr, errorMessage );
            globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
        }
private:
    ISteamNetworkingSockets *clientInstance;
    HSteamNetConnection connection;
    bool shutDown = false;

    void pollIncomingMessages(){
        while(!shutDown){
            ISteamNetworkingMessage *incomingMessage = nullptr;
            int numOfMessages = clientInstance->ReceiveMessagesOnConnection(connection, &incomingMessage, 1);
            if(numOfMessages == 0){
                break;
            }
            if(numOfMessages < 0){
                printf("Error Checking For Messages");
            }
            printf("RECIEVED FROM SERVER");
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
					printf("Local Problem (%s)", connectionInfo->m_info.m_szEndDebug );
				}
				else if (connectionInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally){
					printf("Lost Connection with host (%s)", connectionInfo->m_info.m_szEndDebug );
				}
				else{
					printf("Host Disconnected (%s)", connectionInfo->m_info.m_szEndDebug);
				}

				clientInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false );
				connection = k_HSteamNetConnection_Invalid;
				break;
			}
			case k_ESteamNetworkingConnectionState_Connecting:
				break;
			case k_ESteamNetworkingConnectionState_Connected:
				printf("Connected to server OK");
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
const uint16 DEFAULT_SERVER_PORT = 27020;

int main(int argc, const char *argv[]){
	SteamNetworkingIPAddr serverAddress; 
    serverAddress.Clear();
    gameClient client;
    
    if (!serverAddress.ParseString("127.0.0.1"))
		printf("Invalid server address '%s'", "127.0.0.1");
    serverAddress.m_port = DEFAULT_SERVER_PORT;
    printf("%i",DEFAULT_SERVER_PORT);
    client.InitialiseConnectionSockets();
	client.Run(serverAddress);
	GameNetworkingSockets_Kill();
}