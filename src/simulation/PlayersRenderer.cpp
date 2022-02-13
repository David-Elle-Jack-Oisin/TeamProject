#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif

class PlayersRenderer{
    public:
        void addNewPlayer(Player* player){
            playerList.push_back(player);
            id++;
        }
        void renderPlayers(){
            std::list<Player*>::iterator iter;
            for (iter = playerList.begin(); iter != playerList.end(); ++iter){
                renderPlayer(*iter);
            }
        }
        void loadTexture(){
            Jerry = LoadTexture("JerryXmovement.png");
            frameRec = { 0.0f, 0.0f, (float)Jerry.width/14, (float)Jerry.height};
        }
        void updateSecondPlayer(float x, float y){
            playerList.back()->prevPosition = playerList.back()->position; 
            playerList.back()->position.x = x;
            playerList.back()->position.y = y;
        }
    private:
        std::list<Player*>playerList;
        int id;
        Texture2D Jerry;
        Rectangle frameRec;
       
        void renderPlayer(Player* player){
            DrawTextureRec(Jerry, frameRec, player->position, WHITE);
            // LEFT CHECK
            if (player->prevPosition.x - player->position.x > 0){
                fprintf(stderr,"LEFT");
                player->prevPosition = player->position;
                // ANIMATION CODE

            }
            // RIGHT CHECK 
            if (player->prevPosition.x - player->position.x < 0){
                fprintf(stderr,"RIGHT");
                player->prevPosition = player->position;
                // ANIMATION CODE
            }
            // UP CHECK
            if (player->prevPosition.y - player->position.y > 0){
                fprintf(stderr,"UP");
                player->prevPosition = player->position;
                // ANIMATION CODE
            }
            // DOWN CHECK 
            if (player->prevPosition.y - player->position.y < 0){
                fprintf(stderr,"DOWN");
                player->prevPosition = player->position;
                // ANIMATION CODE

            }
    
        }
};