#include <assert.h>
#include <string.h>
#include <string>
#include <random>
#include <thread>
#include <cstring>
#ifndef _PACKETS_H
#define _PACKETS_H
    #include "packets.cpp"
#endif
#ifndef _ENEMY_RENDERER_H
#define _ENEMY_RENDERER_H
    #include "src/simulation/EnemyRenderer.cpp"
#endif
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
const uint16 DEFAULT_SERVER_PORT = 27020;
class gameClient
{
public:
    void sendPlayerInfo(int id, Vector2 position, int health){
		if (!shutDown) {
        	std::string playerInfoPacket = packets.createPlayerInfoPacket(id, position.x, position.y, health);
			const char *formattedplayerInfoPacket = playerInfoPacket.c_str();
        	clientInstance->SendMessageToConnection(connection, formattedplayerInfoPacket, (uint32)strlen(formattedplayerInfoPacket), k_nSteamNetworkingSend_UnreliableNoDelay, nullptr);
		}
    }
	void sendEnemyInfo(int id, Vector2 position, int health){
		if (!shutDown) {
        	std::string enemyInfoPacket = packets.createEnemyInfoPacket(id, position.x, position.y, health);
			const char *formattedEnemyInfoPacket = enemyInfoPacket.c_str();
        	clientInstance->SendMessageToConnection(connection, formattedEnemyInfoPacket, (uint32)strlen(formattedEnemyInfoPacket), k_nSteamNetworkingSend_UnreliableNoDelay, nullptr);
		}
    }
	void sendDeathPacket(int id){
		if (!shutDown) {
        	std::string playerInfoPacket = packets.createPlayerDeathPacket(id);
			const char *formattedplayerInfoPacket = playerInfoPacket.c_str();
        	clientInstance->SendMessageToConnection(connection, formattedplayerInfoPacket, (uint32)strlen(formattedplayerInfoPacket), k_nSteamNetworkingSend_Reliable, nullptr);
		}
    }
	void startClient(PlayersRenderer *renderer, EnemyRenderer *enemyRender){
		connected = false;
		timeOut = false;
		SteamNetworkingIPAddr serverAddress;
		playRenderer = renderer;
		enemyRenderer = enemyRender;
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
	bool checkTimeOut(){
		return timeOut;
	}
	bool checkConnected(){
		return connected;
	}
private:
	PlayersRenderer *playRenderer;
	EnemyRenderer *enemyRenderer;
    ISteamNetworkingSockets *clientInstance;
    HSteamNetConnection connection;
    bool shutDown = false;
	bool timeOut = false;
	bool connected = false;
	int id;
	float posX, posY;
	int health;
	Packets packets;

	static void InitialiseConnectionSockets(){
        SteamDatagramErrMsg errorMessage;
        GameNetworkingSockets_Init( nullptr, errorMessage );
        SteamNetworkingMicroseconds globalLogTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();
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
			incomingMessage->Release();
			// fprintf(stderr,"NETWORK: Recieved (%s)\n",packet.c_str());
			int typecode = (int)packet.at(0) - 48;
			switch (typecode){
				case 0:{
					std::tie(id, posX, posY) = packets.parseIdReplyPacket(packet);
					playRenderer->matchPlayerIdToServer(id);
					enemyRenderer->setEnemyPosition(posX, posY);
					fprintf(stderr,"NETWORK: Id Recieved (%i)\n",id);
					fprintf(stderr,"NETWORK: EnemyPos (%f, %f)\n",posX,posY);
					break;
				}
				case 1:{
					std::tie(id, posX, posY, health) = packets.parsePlayerInfo(packet);
					if(playRenderer->isNewPlayer(id)){
						Player *ptrPlayer;
						ptrPlayer = new Player(id);
						playRenderer->addNewPlayer(ptrPlayer);
						fprintf(stderr, "ADD NEW PLAYER\n");
					}
					// fprintf(stderr,"NETWORK: OTHER PLAYER INFO(%i, %f:%f,%i)\n", id, posX, posY, health);
					playRenderer->updatePlayerPosition(id, posX, posY);
					break;
				}
				case 2:{
					const char *identifer = packet.substr(packet.find(":") + 1).c_str();
					int id = std::stoi(identifer);
					playRenderer->removePlayer(id);
					fprintf(stderr,"NETWORK: Player (%s) Died\n",identifer);
					break;
				}
				case 3:{
					const char *identifer = packet.substr(packet.find(":") + 1).c_str();
					int id = std::stoi(identifer);
					playRenderer->removePlayer(id);
					fprintf(stderr,"NETWORK: Player (%s) Left\n",identifer);
					break;
				}
				default:
					break;
			}
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
					timeOut = true;
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
				connected = true;
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
			std::string idRequestPacket = packets.createIdRequestPacket();
			const char *formattedidRequestPacket = idRequestPacket.c_str();
			clientInstance->SendMessageToConnection(connection, formattedidRequestPacket, (uint32)strlen(formattedidRequestPacket), k_nSteamNetworkingSend_Reliable, nullptr);
		}
	
	}
};

gameClient *gameClient::gameClientCallBackInstance = nullptr;
