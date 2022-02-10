#include <iostream>
#include <stdio.h>
#include "raylib.h"


class Player 
{
    private:
        #define PLAYER_SPD 100.0f

    public:

    //This makes the walking animation possible
        int framesSpeed = 8;
        int framesCounter = 0;
        int currentFrame = 0;
        

        Vector2 position = {1000, 1000};
        void UpdatePlayer(float delta)
        {   
            Texture2D Jerry = LoadTexture("JerryXmovement.png");    //Sprite Sheet (14 Sprites)
            Rectangle frameRec = { 0.0f, 0.0f, (float)Jerry.width/14, (float)Jerry.height}; //frameRec is Jerry
            DrawTextureRec(Jerry, frameRec, this-> position, WHITE);    //Draw Jerry

            framesCounter++;

            //Player Movement

            if (IsKeyDown(KEY_LEFT)) {
                this->position.x -= PLAYER_SPD*delta;

                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                                            //0-7 is right-facing, 7-14 is left-facing

                        if (currentFrame > 14) {currentFrame = 7;}

                        //This hides the other frames in the animation
                        //So that the walking looks natural

                        frameRec.x = (float)currentFrame*(float)Jerry.width/14;
            } }
            if (IsKeyDown(KEY_RIGHT)) { 
                this->position.x += PLAYER_SPD*delta;
                
                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;

                        if (currentFrame > 7) {currentFrame = 0; }

                            //This hides the other frames in the animation
                            //So that the walking looks natural

                            frameRec.x = (float)currentFrame*(float)Jerry.width/14;
                        }
            }
            if (IsKeyDown(KEY_UP)) {
                this ->position.y -= PLAYER_SPD*delta;
                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;

                        if (currentFrame > 7) {currentFrame = 0;}

                            //This hides the other frames in the animation
                            //So that the walking looks natural

                            frameRec.x = (float)currentFrame*(float)Jerry.width/14;
                        }
            }
            if (IsKeyDown(KEY_DOWN)) {
                this ->position.y += PLAYER_SPD*delta;
                if (framesCounter >= (60/framesSpeed))
                    {
                        framesCounter = 0;
                        currentFrame++;

                        if (currentFrame > 14) {currentFrame = 7;}

                            //This hides the other frames in the animation
                            //So that the walking looks natural

                            frameRec.x = (float)currentFrame*(float)Jerry.width/14;
                        }
            }
            
            };
        };
