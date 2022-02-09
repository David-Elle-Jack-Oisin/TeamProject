#include <iostream>
#include <stdio.h>
#include "raylib.h"
#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1

class Player 
{
    private:
        #define PLAYER_SPD 100.0f
    public:
        int currentFrame = 0;
    
    //This makes the walking animation possible
        int framesSpeed = 8;
        int framesCounter = 0;

        Vector2 position = {1000, 1000};
        void UpdatePlayer(float delta)
        {   
            Texture2D Jerry = LoadTexture("JerryWalking.png");
            Rectangle frameRec = { 0.0f, 0.0f, (float)Jerry.width/7, (float)Jerry.height/2 };
            DrawTextureRec(Jerry, frameRec, this-> position, WHITE);        

            if (IsKeyDown(KEY_LEFT)) {
                this->position.x -= PLAYER_SPD*delta;
                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;

                        if (currentFrame > 1) currentFrame = 0;

                        //This hides the other frames in the animation
                        //So that the walking looks natural

                        frameRec.x = (float)currentFrame*(float)Jerry.width/5;
            }
            if (IsKeyDown(KEY_RIGHT)) { 
                this ->position.x += PLAYER_SPD*delta;
                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;
                        if (currentFrame > 1) currentFrame = 0; //NEED to change the numbers to apply to jerry spritesheet

                            //This hides the other frames in the animation
                            //So that the walking looks natural

                            frameRec.x = (float)currentFrame*(float)Jerry.width/5;
                        }
            }
            if (IsKeyDown(KEY_UP)) this ->position.y -= PLAYER_SPD*delta;
            if (IsKeyDown(KEY_DOWN)) this-> position.y += PLAYER_SPD*delta;
            };
        };
};