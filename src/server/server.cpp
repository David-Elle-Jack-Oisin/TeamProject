#include <assert.h>
#include <chrono>
#include <thread>
#include <map>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif
SteamNetworkingMicroseconds globalLogTimeZero;
class gameServer{
    public:
        void Run(uint16 port){
            serverInstance = SteamNetworkingSockets();
            SteamNetworkingIPAddr serverLocalAddress;
            serverLocalAddress.Clear();
            serverLocalAddress.m_port = port;
            SteamNetworkingConfigValue_t serverConfig;
            serverConfig.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChanged );
            listenSocket = serverInstance->CreateListenSocketIP( serverLocalAddress, 1, &serverConfig );
            if ( listenSocket == k_HSteamListenSocket_Invalid )
                printf( "Failed to listen on port %d", port );SteamNetworkingMicroseconds globalLogTimeZero;
            serverPollGroup = serverInstance->CreatePollGroup();
            if ( serverPollGroup == k_HSteamNetPollGroup_Invalid )
                printf( "Failed to listen on port %d", port );
            printf( "Server listening on port %d\n", port );

            while(!shutDown){
                pollIncomingMessages();
                pollConnectionStateChanges();
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            }
            printf( "Closing connections...\n" );
            for (auto client: clients){
                SendToClient(client.first, "Shutdown");
                serverInstance->CloseConnection(client.first, 0, "Server Shutdown", true);
            }
            clients.clear();
            serverInstance->CloseListenSocket( listenSocket );
		    listenSocket = k_HSteamListenSocket_Invalid;

		    serverInstance->DestroyPollGroup( serverPollGroup );
		    serverPollGroup = k_HSteamNetPollGroup_Invalid;
        }
        static void InitialiseConnectionSockets(){
            SteamDatagramErrMsg errorMessage;
            GameNetworkingSockets_Init( nullptr, errorMessage );
            globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
        }
    private:
        struct Client{
            int posx;
            int posy;
	    };
        bool shutDown = false;
        ISteamNetworkingSockets *serverInstance;
	    HSteamNetPollGroup serverPollGroup;
        HSteamListenSocket listenSocket;
        std::map< HSteamNetConnection, Client > clients;

        void SendToClient( HSteamNetConnection connection, const char *str ){
		    serverInstance->SendMessageToConnection( connection, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr );
	    }

        void SendToAllClients( const char *str, HSteamNetConnection exception = k_HSteamNetConnection_Invalid ){
            for ( auto &client: clients ){
                if ( client.first != exception )
                    SendToClient( client.first, str );
            }
        }

        static gameServer *gameServerCallBackInstance;

        static void ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *connectionInfo){
            gameServerCallBackInstance->OnConnectionStatusChanged(connectionInfo);
        }
        void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *connectionInfo){
            char temp[1024];
            // depends on connection state 
            switch (connectionInfo->m_info.m_eState){
                case k_ESteamNetworkingConnectionState_None:
                    break;
                case k_ESteamNetworkingConnectionState_ClosedByPeer:
			    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:{
                    // only if it was actually connected
                    if (connectionInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected){
                        auto client = clients.find(connectionInfo->m_hConn);
                        assert(client != clients.end());
                        const char *debugMessage;
                        if (connectionInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally){
                            debugMessage = "problem detected locally";
                            // send to client
                            sprintf(temp,"Client Occured A Problem And Disconnected (%s)", connectionInfo->m_info.m_szEndDebug);
                        }
                        else{
                            debugMessage = "closed by peer";
                            sprintf(temp, "Client Left");
                        }
                        printf("Connection %s %s, reason %d: %s\n",
                            connectionInfo->m_info.m_szConnectionDescription,
                            debugMessage,
                            connectionInfo->m_info.m_eEndReason,
                            connectionInfo->m_info.m_szEndDebug
                        );
                        clients.erase(client);
                        SendToAllClients(temp);

                    }
                    else{
                        assert(connectionInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
                    }
                    serverInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false);
                    break;
                }
                case k_ESteamNetworkingConnectionState_Connecting:
                    assert(clients.find(connectionInfo->m_hConn) == clients.end());
                    printf("Connection request from %s", connectionInfo->m_info.m_szConnectionDescription);
                    if(serverInstance->AcceptConnection(connectionInfo->m_hConn) != k_EResultOK){
                        serverInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false);
                        printf("Can't Accept Connection");
                        break;
                    }

                    if(!serverInstance->SetConnectionPollGroup(connectionInfo->m_hConn, serverPollGroup)){
                        serverInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false);
                        printf("Failed To Set Poll Group");
                        break;
                    }

                    sprintf(temp, "Welcome");
                    SendToClient(connectionInfo->m_hConn, temp);
                    sprintf(temp, "New Client");
                    SendToAllClients(temp, connectionInfo->m_hConn);
                    clients[connectionInfo->m_hConn];
                    break;
                case k_ESteamNetworkingConnectionState_Connected:
                    break;
                default:
                    break;
            }
        }
        void pollIncomingMessages(){
            char temp[1024];
            while(!shutDown){
                ISteamNetworkingMessage *incomingMessage = nullptr;
                int numOfMessages = serverInstance->ReceiveMessagesOnPollGroup(serverPollGroup, &incomingMessage, 1);
                if(numOfMessages == 0){
                    break;
                }
                if(numOfMessages < 0){
                    printf("Error Checking For Messages");
                }
                assert(numOfMessages == 1 && incomingMessage);
                auto client = clients.find( incomingMessage->m_conn);
                assert(client != clients.end());

                std::string packet;
			    packet.assign((const char *)incomingMessage->m_pData, incomingMessage->m_cbSize);
			    const char *formattedPacket = packet.c_str();

                incomingMessage->Release();

                sprintf(temp, formattedPacket);
                SendToAllClients(temp, client->first);

            }
        }
        void pollConnectionStateChanges(){
            gameServerCallBackInstance = this;
            serverInstance->RunCallbacks();
        }
};

const uint16 DEFAULT_SERVER_PORT = 27020;

int main( int argc, const char *argv[] )
{
	int port = DEFAULT_SERVER_PORT;
	SteamNetworkingIPAddr serverAddress; serverAddress.Clear();
    gameServer server;

	// Create client and server sockets
	server.InitialiseConnectionSockets();
	server.Run((uint16)port );
	GameNetworkingSockets_Kill();

}