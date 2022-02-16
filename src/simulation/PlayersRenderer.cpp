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
            Jerry = LoadTexture("smallJerry.png");
            frameRec = { 0.0f, 0.0f, (float)Jerry.width/16, (float)Jerry.height};
        }
        void updateSecondPlayer(float x, float y){
            playerList.back()->prevPosition = playerList.back()->position; 
            playerList.back()->position.x = x;
            playerList.back()->position.y = y;
        }
        void matchPlayerIdToServer(int id){
            playerList.front()->matchIdToServer(id);
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
                player->prevPosition = player->position;

                // ANIMATION CODE
                player->framesCounter++;
                if (player->framesCounter >= (60/player->framesSpeed)){
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing
                        if (player->currentFrame > 15) {player->currentFrame = 8;}
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 
            }
            // RIGHT CHECK 
            if (player->prevPosition.x - player->position.x < 0){
                player->prevPosition = player->position;
                // ANIMATION CODE
                player->framesCounter++;
                if (player->framesCounter >= (60/player->framesSpeed))
                {
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing

                        if (player->currentFrame > 7) {player->currentFrame = 1;}  
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 
            }
            // UP CHECK
            if (player->prevPosition.y - player->position.y > 0){
                player->prevPosition = player->position;
                // ANIMATION CODE

            }
            // DOWN CHECK 
            if (player->prevPosition.y - player->position.y < 0){
                player->prevPosition = player->position;
                // ANIMATION CODE

            }
    
        }
};