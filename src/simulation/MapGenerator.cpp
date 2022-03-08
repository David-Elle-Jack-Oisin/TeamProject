#include <iostream>
#include <stdio.h>
#include "raylib.h"
#ifndef _MAP_H
#define _MAP_H
    #include "MapGenerator.cpp"
#endif
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif





// Vector2 backgroundPosition = (backgroundX, backgroundY);
// Texture2D background = LoadTexture("src/src-sprites/Eyes.png");
// Rectangle backgroundFrameRec = {0.0f, 0.0f, (float)backgroundWidth/2, (float)backgroundWidth/3}

class BossRoom{
    private:

    public:
    
    int backgroundCurrentFrame = 0;
    int backgroundFramesCounter = 0;
    int backgroundFramesSpeed = 7; //(May need to be adjusted)
    int backgroundY = 200;
    int backgroundX = 1000;

    Rectangle topNoEntrance;

};

class StartingRoom{
    private:
        Texture2D Stone;
        Texture2D Stone2;
        Texture2D Stone3;
        Texture2D Floor;
        Texture2D Bones;
        Texture2D EyesLeft;
        Texture2D EyesRight;
        int frameCounter;
        Rectangle eyesFrameRec;
        int currentFrame;
        int stareCounter;
    public:

    Rectangle Top = {0, 0, 1920, 100};
    Rectangle RSide = {1820, 0, 100, 1080};
    Rectangle LSide = {0, 0, 100, 1080};
    Rectangle Bottom = {0, 980, 1920, 100};
    Rectangle Center = {100, 100, 1720, 880};
    Rectangle test = {0, 0, 150, 150};

    Rectangle rectangles[6] = {Top, RSide, Center, LSide, Bottom, test};

    void loadTextures(){
        Bones = LoadTexture("src-sprites/bonePile.png");
        Stone = LoadTexture("src-sprites/mossyStones.png");
        Stone2 = LoadTexture("src-sprites/Stone2.png");
        Stone3 = LoadTexture("src-sprites/Stone3.png");
        Floor = LoadTexture("src-sprites/floor.png");
        EyesLeft = LoadTexture("src-sprites/EyesLookLeft.png");
        EyesRight = LoadTexture("src-sprites/EyesLookingRight.png");
        eyesFrameRec = { 0.0f, 0.0f, (float)EyesLeft.width/17, (float)EyesLeft.height};
        SetShapesTexture(Bones, test);
        SetShapesTexture(Floor, Center);
        SetShapesTexture(Stone2, RSide);
        SetShapesTexture(Stone2, Top);
        SetShapesTexture(Stone2, Bottom);
        SetShapesTexture(Stone2, LSide);

    }
    
    int Draw(){
        DrawTextureRec(Floor, Center, {100,100}, WHITE);
        DrawTextureRec(Stone2, RSide, {1820,0}, WHITE);
        DrawTextureRec(Stone2, Top, {0,0}, WHITE);
        DrawTextureRec(Stone2, Bottom, {0,980}, WHITE);
        DrawTextureRec(Stone2, LSide, {0, 0}, WHITE);
        DrawTextureRec(Bones, test, {1600, 100}, WHITE);
        DrawTextureRec(Bones, test, {900, 400}, WHITE);
        DrawTextureRec(Bones, test, {1100, 300}, WHITE);
        DrawTextureRec(Bones, test, {200, 700}, WHITE);
        DrawTextureRec(Bones, test, {400, 600}, WHITE);
        DrawTextureRec(Bones, test, {500, 100}, WHITE);
        DrawTextureRec(Bones, test, {1300, 700}, WHITE);
        DrawTextureRec(Bones, test, {700, 200}, WHITE);
        DrawTextureRec(EyesLeft, eyesFrameRec, {2000, 400}, WHITE);
        DrawTextureRec(EyesLeft, eyesFrameRec, {2100, 900}, WHITE);
        DrawTextureRec(EyesLeft, eyesFrameRec, {2300, 100}, WHITE);
        DrawTextureRec(EyesLeft, eyesFrameRec, {1100, -100}, WHITE);
        DrawTextureRec(EyesLeft, eyesFrameRec, {1400, 1100}, WHITE);
        DrawTextureRec(EyesRight, eyesFrameRec, {-200, -200}, WHITE);
        DrawTextureRec(EyesRight, eyesFrameRec, {350, -100}, WHITE);
        DrawTextureRec(EyesRight, eyesFrameRec, {-140, 300}, WHITE);
        DrawTextureRec(EyesRight, eyesFrameRec, {-190, 800}, WHITE);
        DrawTextureRec(EyesRight, eyesFrameRec, {200, 1200}, WHITE);
        frameCounter++;
        if (frameCounter >= 60/8){
            frameCounter = 0;
            if (currentFrame != 11 && currentFrame != 0){
                currentFrame++;     //currentFrame is the current image of Jerry from the spritesheet
                if (currentFrame > 17) {currentFrame = 0;}
                    //This hides the other frames in the animation
                    //So that the walking looks natural
                    eyesFrameRec.x = (float)currentFrame*(float)EyesLeft.width/17;
            }
            else{
                stareCounter++;
                if (stareCounter >= 20){
                    stareCounter = 0;
                    currentFrame++;
                }
            }
            
        }
        return 1;
    };

};




StartingRoom startingRoom;

class MapGenerator {
    private:

    public:

    // Two Dimensional Array For Terrain Generation Spans 1 Screen

    int x;
    int y;
    int height = 11;
    int length = 20;


// ================================ GRID MAP ================================

    int oneScreenMap [11][20] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
                                 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

// ================================= GRID MAP DRAW ===============================

    void GridDrawMap(){
        for (y = 0; y <= height; ++y){
            for (x = 0; x <= length; ++x){

                Rectangle rect = {(float)(0 + (80 * x)), (float)(20 + (80 * y)), 80, 80};

                if (oneScreenMap[y][x] == 1){
                    DrawRectangleRec(rect, GRAY);
                }

                if (oneScreenMap[y][x] == 2){
                    DrawRectangleRec(rect, BLACK);
                }
            }
        }
    }


// =================================== GRID MAP DRAW ==============================



// =================================== LIST MAP DRAW ==============================
    // int MapTerrainLength = 5;

    // typedef struct Terrain{
    //     Rectangle Rectangle;
    //     Color color;
    // };

    // Terrain MapTerrain[] ={ 
    // {{0, 0, 1000, 400}, BLACK},
    // {{0, 0, 1000, 400}, BLACK}
    // };

    // int ListDrawMap(){
    //     for (int i = 0; i < MapTerrainLength; i++) {DrawRectangleRec(MapTerrain[i].Rectangle, MapTerrain[i].color);}
    // };

// =================================== LIST MAP DRAW ===============================


// =================================== CLASS MAP DRAW ===============================

    void ClassDrawMap(){
        startingRoom.Draw();
    }

// ==================================== Directional Collisions ===========================


   // bool collideFromBottom(Player *player){
    //     return player->prevHitboxPosition.y  >= startingRoom.Top.y + startingRoom.Top.height && player->hitboxPosition.y 
    //     <= startingRoom.Top.y + startingRoom.Top.height;  
    // }
         // I think prevHitboxPosition and hitboxPosition are not properly being reset to pre collision states


            // If hit from bottom



    // bool collideFromLeft(Player *player){
    //     return player->prevHitboxPosition.x + player->hitBox.width < startingRoom.Top.x && player->hitboxPosition.x + player->hitBox.width >= startingRoom.Top.x;  
    // }

    // bool collideFromRight(Player *player){
    //     return player->prevHitboxPosition.x >= startingRoom.Top.x + startingRoom.Top.width && player->hitboxPosition.x < startingRoom.Top.x +startingRoom.Top.width;
    // }   

    // bool collideFromTop(Player *player){
    //     return player->prevHitboxPosition.y + player->hitBox.height < startingRoom.Top.y && player->hitboxPosition.y + player->hitBox.height >= startingRoom.Top.y;  
    // }

    // bool collideFromBottom(Player *player){
    //     return player->prevHitboxPosition.y  >= startingRoom.Top.y + startingRoom.Top.height && player->hitboxPosition.y <= startingRoom.Top.y + startingRoom.Top.height;  
    // }

    //             if(collideFromLeft(player)){
    //             // player->position.x = startingRoom.Top.x -1;
    //             // player->hitboxPosition.x = startingRoom.Top.x - 1;
    //         }

    //         if(collideFromRight(player)){
    //             fprintf(stderr,"right Collision\n");
    //         }

    //         if(collideFromTop(player)){
    //             fprintf(stderr,"Top Collision\n");
    //             fprintf(stderr, "%i", player->hitboxPosition.x);
    //         }

    //         // fprintf(stderr, "%i\n", player->prevPosition.x);
    //        // fprintf(stderr, "%i\n", player->prevPosition.y);

            // if(collideFromBottom(player)){
            //     fprintf(stderr,"Bottom Collision\n");


                
            //     //player->hitboxPosition.y = startingRoom.Top.height + startingRoom.Top.y ;
            //     //player->prevHitboxPosition.y = startingRoom.Top.height + startingRoom.Top.y;

            //     // Resolves collision
            //    player->position.y = startingRoom.Top.height + startingRoom.Top.y + 1;
            //    player->prevHitboxPosition.y = player->hitboxPosition.y;
            //    player->hitboxPosition.y = startingRoom.Top.y + startingRoom.Top.height + 1;
                

            // }

//   =========================================================================================================

    void CheckCollisionTop(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.Top))){
            //fprintf(stderr,"Collision\n");
            // DrawRectangleRec(player->frameRec, RED);
            // DrawRectangleRec(player->hitBox, GREEN);

            if(player->hitBox.y < (startingRoom.Top.height + startingRoom.Top.y) && player ->hitBox.x  > startingRoom.Top.x && player ->hitBox.x < (startingRoom.Top.x + startingRoom.Top.width)){
                
                player->position.y = startingRoom.Top.height + startingRoom.Top.y - 11;
                player->positionOffset = {player->position.x - 100 , player->position.y - 100};
                fprintf(stderr,"Up Collision\n");
            }
        }
    }

    void CheckCollisionBottom(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.Bottom))){
            //fprintf(stderr,"Collision\n");
            // DrawRectangleRec(player->frameRec, RED);
            // DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if((player->hitBox.y + player->hitBox.height) > (startingRoom.Bottom.y)){
                fprintf(stderr, "Bottom Collision");
                player->position.y  = (startingRoom.Bottom.y - player->hitBox.height - 11);
                player->positionOffset = {player->position.x - 100 , player->position.y - 100};
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    void CheckCollisionLeft(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.LSide))){
            //fprintf(stderr,"Collision\n");
            // DrawRectangleRec(player->frameRec, RED);
            // DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if((player->hitBox.x  < (startingRoom.LSide.x + startingRoom.LSide.width))){
                fprintf(stderr, "Left Collision");
                player->position.x  = (startingRoom.LSide.x + 70);
                player->positionOffset = {player->position.x - 100 , player->position.y - 100};
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    


    void CheckCollisionRight(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.RSide))){
            //fprintf(stderr,"Collision\n");
            // DrawRectangleRec(player->frameRec, RED);
            // DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if(((player->hitBox.x + player->hitBox.width) > startingRoom.RSide.x)){
                fprintf(stderr, "Left Collision");
                player->position.x  = (startingRoom.RSide.x - 55);
                player->positionOffset = {player->position.x - 100 , player->position.y - 100};
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    void CheckCollision(Player *player){
        CheckCollisionTop(player);
        CheckCollisionBottom(player);
        CheckCollisionLeft(player);
        CheckCollisionRight(player);
    }

    void loadTextures(){
        startingRoom.loadTextures();
    }

};


