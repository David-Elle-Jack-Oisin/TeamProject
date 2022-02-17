#include <assert.h>
#include <chrono>
#include <thread>
#include <map>
#include "packets.cpp"
#include <string.h>
#include <string>

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
            if(listenSocket == k_HSteamListenSocket_Invalid){
                fprintf(stderr, "Failed to listen on port %d\n", port );
            }
            serverPollGroup = serverInstance->CreatePollGroup();
            if ( serverPollGroup == k_HSteamNetPollGroup_Invalid )
                fprintf(stderr, "Failed to listen on port %d\n", port );
            fprintf(stderr, "Server listening on port %d\n", port );

            while(!shutDown){
                pollIncomingMessages();
                pollConnectionStateChanges();
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            }
            fprintf(stderr, "Closing connections...\n" );
            for (auto client: clients){
                SendToClient(client.first, "Shutdown");
                serverInstance->CloseConnection(client.first, 0, "Server Shutdown\n", true);
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
        Packets packets;
        int id = 0;

        void SendToClient( HSteamNetConnection connection, const char *str ){
		    serverInstance->SendMessageToConnection( connection, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr );
	    }

        void SendToAllClients( const char *str, HSteamNetConnection exception = k_HSteamNetConnection_Invalid ){
            for ( auto &client: clients ){
                if ( client.first != exception )
                    SendToClient( client.first, str );
            }
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
                        
                        fprintf(stderr,"NETWORK: Connection %s %s, reason %d: %s\n",
                            connectionInfo->m_info.m_szConnectionDescription,
                            debugMessage,
                            connectionInfo->m_info.m_eEndReason,
                            connectionInfo->m_info.m_szEndDebug
                        );
                        id--;
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
                    fprintf(stderr,"NETWORK: Connection request from %s\n", connectionInfo->m_info.m_szConnectionDescription);
                    if(serverInstance->AcceptConnection(connectionInfo->m_hConn) != k_EResultOK){
                        serverInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false);
                        fprintf(stderr,"NETWORK: Can't Accept Connection\n");
                        break;
                    }

                    if(!serverInstance->SetConnectionPollGroup(connectionInfo->m_hConn, serverPollGroup)){
                        serverInstance->CloseConnection(connectionInfo->m_hConn, 0, nullptr, false);
                        fprintf(stderr,"NETWORK: Failed To Set Poll Group\n");
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
                    fprintf(stderr,"NETWORK: Error Checking For Messages\n");
                }
                assert(numOfMessages == 1 && incomingMessage);
                auto client = clients.find( incomingMessage->m_conn);
                assert(client != clients.end());

                std::string packet;
			    packet.assign((const char *)incomingMessage->m_pData, incomingMessage->m_cbSize);
			    const char *formattedPacket = packet.c_str();
                fprintf(stderr, "RECEIEVED: %s\n", formattedPacket );
                incomingMessage->Release();
                if (!packet.empty()){
                    char typecode = packet.at(0);
                    fprintf(stderr, "TYPECODE: %c\n", typecode );
                    if (typecode == '0'){
                        fprintf(stderr, "ID REQUEST: %s\n", formattedPacket );
                        std::string idPacket = packets.createIdReplyPacket(id);
                        const char *formattedidPacket = idPacket.c_str();
                        SendToClient(client->first, formattedidPacket);
                        id++;
                    }
                    else{
                        sprintf(temp, "%s", formattedPacket);
                        SendToAllClients(temp, client->first);
                    }
                }
               
            }
        }

        static gameServer *gameServerCallBackInstance;

        static void ConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *connectionInfo){
            gameServerCallBackInstance->OnConnectionStatusChanged(connectionInfo);
        }
        void pollConnectionStateChanges(){
            gameServerCallBackInstance = this;
            serverInstance->RunCallbacks();
        }
};

gameServer *gameServer::gameServerCallBackInstance = nullptr;
const uint16 DEFAULT_SERVER_PORT = 27020;

int main( int argc, const char *argv[] )
{
	int port = DEFAULT_SERVER_PORT;
	SteamNetworkingIPAddr serverAddress; serverAddress.Clear();
    gameServer server;

	// create server sockets
	server.InitialiseConnectionSockets();
	server.Run((uint16)port );
	GameNetworkingSockets_Kill();

}