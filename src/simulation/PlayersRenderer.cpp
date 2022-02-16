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

class PlayersRenderer{
    public:
        void addNewPlayer(Player* player){
            playerList.push_back(player);
            id++;
            player->currentHealthFrame = 1;
            player->playerHealth = 9;     
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

            Health = LoadTexture("Health.png");
            healthFrameRec = { 0.0f, 0.0f, (float)Health.width/11 , (float)Health.height};
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

        Texture2D Health;
        Rectangle healthFrameRec;

       
        void renderPlayer(Player* player){
            renderHealth(player);

            DrawTextureRec(Jerry, frameRec, player->position, WHITE);
            DrawTextureRec(Health, healthFrameRec, player->position, WHITE);  
    
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

        void renderHealth(Player* player) {

            if (IsKeyPressed(80)) {     //(if P is pressed), will replace with collisions later

                fprintf(stderr,"HEALTH: %i\n", player->playerHealth);
                healthFrameRec.x = (float)player->currentHealthFrame*(float)Health.width/11;
                player->currentHealthFrame++;

                player->playerHealth--;
            }
        }
};