#include <assert.h>
#include <chrono>
#include <thread>
#include <map>
#ifndef _PACKETS_H
#define _PACKETS_H
    #include "packets.cpp"
#endif
#include <string.h>
#include <string>
#include <atomic>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

class gameServer{
    public:
        void startServer(){
            int port = DEFAULT_SERVER_PORT;
            SteamNetworkingIPAddr serverAddress; serverAddress.Clear();
            gameServer server;
            // create server sockets
            server.InitialiseConnectionSockets();
            server.Run((uint16)port);
            if (serverShutDown.load()){
                GameNetworkingSockets_Kill();
            }
            
        }
        void turnOff(){
		    serverShutDown.store(true);
	    }
        gameServer():serverShutDown(false){}
    private:
        struct Client{
            int posx;
            int posy;
	    };
        int EnemyId;
        float EnemyPosX;
        float EnemyPosY;
        int EnemyHealth;
        float enemyPosX = 1400;
        float enemyPosY = 540;
        int enemyHealth;
        std::atomic<bool> serverShutDown;
        ISteamNetworkingSockets *serverInstance;
	    HSteamNetPollGroup serverPollGroup;
        HSteamListenSocket listenSocket;
        std::map< HSteamNetConnection, Client > clients;
        std::map< HSteamNetConnection, int > clientToIdMap;
        Packets packets;
        const uint16 DEFAULT_SERVER_PORT = 27020;
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
    
            while(!serverShutDown.load()){
                pollIncomingMessages();
                pollConnectionStateChanges();
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
                // fprintf(stderr, "%i", std::this_thread::get_id());
            }
            fprintf(stderr, "Closing connections...\n" );
            for (auto client: clients){
                SendToClient(client.first, "serverShutDown");
                serverInstance->CloseConnection(client.first, 0, "Server serverShutDown\n", true);
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
            SteamNetworkingMicroseconds globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
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
                        std::string disconnectPacket = packets.createPlayerDisconnectPacket(clientToIdMap.at(client->first));
                        const char *formattedDisconnectPacket = disconnectPacket.c_str();
                        clientToIdMap.erase(client->first);
                        SendToAllClients(formattedDisconnectPacket);

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
            // fprintf(stderr, "%i", serverShutDown.load());
            while(!serverShutDown.load()){
                ISteamNetworkingMessage *incomingMessage = nullptr;
                int numOfMessages = serverInstance->ReceiveMessagesOnPollGroup(serverPollGroup, &incomingMessage, 1);
                if(numOfMessages == 0){
                    break;
                }
                if(numOfMessages < 0){
                    fprintf(stderr,"NETWORK: Error Checking For Messages\n");
                    serverShutDown = true;
                }
                
                if(numOfMessages == 1 && incomingMessage){
                    auto client = clients.find( incomingMessage->m_conn);
                    
                    if(client != clients.end()){

                        std::string packet;
                        packet.assign((const char *)incomingMessage->m_pData, incomingMessage->m_cbSize);
                        const char *formattedPacket = packet.c_str();
                        // fprintf(stderr, "RECEIEVED: %s\n", formattedPacket );
                        incomingMessage->Release();
                        if (!packet.empty()){
                            int typecode = (int)packet.at(0) - 48;
			                switch (typecode){
                                case 0:{
                                    fprintf(stderr, "ID REQUEST: %s\n", formattedPacket );
                                    std::string idPacket = packets.createIdReplyPacket(id, enemyPosX, enemyPosY);
                                    clientToIdMap.insert({client->first, id});
                                    const char *formattedidPacket = idPacket.c_str();
                                    SendToClient(client->first, formattedidPacket);
                                    id++;
                                    break;
                                }
                                case 6:{
                                    fprintf(stderr, "ENEMY: %s\n", formattedPacket );
                                    std::tie(EnemyId, EnemyPosX, EnemyPosY, EnemyHealth) = packets.parseEnemyInfo(packet);
                                    if (EnemyPosX != enemyPosX) enemyPosX = EnemyPosX;
                                    if (EnemyPosY != enemyPosY) enemyPosY = EnemyPosY;
                                    break;
                                }
                                default:{
                                    sprintf(temp, "%s", formattedPacket);
                                    SendToAllClients(temp, client->first);
                                }
                            }
                        }
                    }
                }
                else{
                    serverShutDown = true;
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