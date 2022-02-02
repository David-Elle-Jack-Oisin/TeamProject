
#include <map>
#include <chrono>
#include <thread>

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
                printf( "Failed to listen on port %d", port );
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

        static void InitialiseConnectionSockets(){
            SteamDatagramErrMsg errorMessage;
            GameNetworkingSockets_Init( nullptr, errorMessage );
            globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
        }
        void SendToClient( HSteamNetConnection connection, const char *str ){
		    serverInstance->SendMessageToConnection( connection, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr );
	    }

        void SendToAllClients( const char *str, HSteamNetConnection exception = k_HSteamNetConnection_Invalid ){
            for ( auto &client: clients ){
                if ( client.first != exception )
                    SendToClient( client.first, str );
            }
        }
};