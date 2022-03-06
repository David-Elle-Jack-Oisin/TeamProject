#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include "raylib.h"
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif
#ifndef _ENEMY_RENDERER_H
#define _ENEMY_RENDERER_H
    #include "EnemyRenderer.cpp"
#endif


class PlayersRenderer{
    public:

        void addNewPlayer(Player* player){
            playerMap.insert({player->id, player});
            player->currentHealthFrame = 0;
            player->playerHealth = 10;
            player->frameRec = { 0.0f, 0.0f, (float)Jerry.width/16, (float)Jerry.height};
            player->healthFrameRec = { 0.0f, 0.0f, (float)Health.width/11 , (float)Health.height};
            player->hitBox = { (player->position.x + 50), (player->position.y + 30), 26, 65};
            player->positionOffset = {player->position.x - 100 , player->position.y - 100};
        }
        void removePlayer(int id){
            playerMap.erase(id);     
        }
        void renderPlayers(){
            std::map<int, Player*>::iterator iter;
            for (iter = playerMap.begin(); iter != playerMap.end(); ++iter){
                renderPlayer(iter->second);
            }
        }
        void loadTexture(){
            Jerry = LoadTexture("src-sprites/smallJerry.png");
            Health = LoadTexture("src-sprites/Health.png");
        }
        void updatePlayerPosition(int id, float x, float y){
            playerMap.at(id)->prevPosition = playerMap.at(id)->position; 
            playerMap.at(id)->position.x = x;
            playerMap.at(id)->position.y = y;
            playerMap.at(id)->positionOffset.x = x - 100;
            playerMap.at(id)->positionOffset.y = y - 100;
        }
        void matchPlayerIdToServer(int id){
            Player* player = playerMap.at(30);
            playerMap.erase(30);
            player->matchIdToServer(id);
            playerMap.insert({player->id, player});
        }
        bool isNewPlayer(int id){
            return !(playerMap.count(id) > 0);
        }
        PlayersRenderer(std::map<int, Player*>&map):playerMap(map){}
    private:
        std::map<int, Player*>&playerMap;
        Texture2D Jerry;
        int LastKey;
        Texture2D Health;


        // bool directionRight(Player* player){
        //     if(player->prevPosition.x - player->position.x < 0){
        //         return true;
        //     }
        //     return false;
        // }

       
        void renderPlayer(Player* player){
            renderHealth(player);

            DrawTextureRec(Jerry, player->frameRec, player->position, WHITE);
            DrawTextureRec(Health, player->healthFrameRec, player->position, WHITE);
            int lastPosX = player->hitBox.x;  
            
            // LEFT CHECK
            if (player->prevPosition.x - player->position.x > 0){
                player->playerXDir = player->prevPosition.x - player->position.x;
                player->playerYDir = 0.0;
                player->prevPosition = player->position;
                LastKey = 1;

                // ANIMATION CODE
                player->framesCounter++;
                if (player->framesCounter >= (60/player->framesSpeed)){
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing
                        if (player->currentFrame > 15) {player->currentFrame = 8;}
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 
            }
            // RIGHT CHECK 
            if (player->prevPosition.x - player->position.x < 0){
                player->playerXDir = player->prevPosition.x - player->position.x;
                player->playerYDir = 0.0;
                player->prevPosition = player->position;
                LastKey = 2;
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
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 
            }
            // UP CHECK
            if (player->prevPosition.y - player->position.y > 0){
                player->playerYDir = player->prevPosition.y - player->position.y;
                player->playerXDir = 0.0;
                player->prevPosition = player->position;

                // ANIMATION CODE  LEFT SIDE
                player->framesCounter++;
                if ((player->framesCounter >= (60/player->framesSpeed)) && LastKey == 1){
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing
                        if (player->currentFrame > 15) {player->currentFrame = 8;}
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 

                            if(lastPosX - player->hitBox.x > 0){
                LastKey = 2;
            }
                // ANIMATION CODE RIGHT SIDE
                if ((player->framesCounter >= (60/player->framesSpeed) && LastKey == 2))
                {
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing

                        if (player->currentFrame > 7) {player->currentFrame = 1;}  
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 



            }
            // DOWN CHECK 
            if (player->prevPosition.y - player->position.y < 0){
                player->playerYDir = player->prevPosition.y - player->position.y;
                player->playerXDir = 0.0;
                player->prevPosition = player->position;
                // ANIMATION CODE

                // ANIMATION CODE  LEFT SIDE
                player->framesCounter++;
                if ((player->framesCounter >= (60/player->framesSpeed)) && LastKey == 1){
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing
                        if (player->currentFrame > 15) {player->currentFrame = 8;}
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                } 

                // ANIMATION CODE RIGHT SIDE
                if ((player->framesCounter >= (60/player->framesSpeed)) && LastKey == 2)
                {
                        player->framesCounter = 0;
                        player->currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing

                        if (player->currentFrame > 7) {player->currentFrame = 1;}  
                        //This hides the other frames in the animation
                        //So that the walking looks natural
                        player->frameRec.x = (float)player->currentFrame*(float)Jerry.width/16;
                }

            }
    
        }

        void renderHealth(Player* player) {
            player->healthFrameRec.x = (float)player->currentHealthFrame*(float)Health.width/11;
        }
};