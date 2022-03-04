#include <iostream>
#include <stdio.h>
#include "raylib.h"
#ifndef _MAP_H
#define _MAP_H
    #include "MapGenerator.cpp"
#endif


class BossRoom{
    private:

    public:

    Rectangle topNoEntrance;

};

class StartingRoom{
    private:

    public:

    Rectangle Top = {0, 0, 1920, 100};
    Rectangle RSide = {1820, 0, 100, 1080};
    Rectangle LSide = {0, 0, 100, 1080};
    Rectangle Bottom = {0, 980, 1920, 100};
    Rectangle Center = {100, 100, 1720, 880};

    Rectangle rectangles[5] = {Top, RSide, Center, LSide, Bottom};

    int Draw(){
        Texture2D Stone;
        Texture2D Stone2;
        Texture2D Stone3;
        Stone = LoadTexture("src-sprites/mossyStones.png");
        Stone2 = LoadTexture("src-sprites/Stone2.png");
        Stone3 = LoadTexture("src-sprites/Stone3.png");
        

        SetShapesTexture(Stone3, Center);
        DrawTextureRec(Stone3, Center, {100,100}, WHITE);

        SetShapesTexture(Stone, RSide);
        DrawTextureRec(Stone, RSide, {100,100}, WHITE);

        SetShapesTexture(Stone, Top);
        DrawTextureRec(Stone, Top, {100,100}, WHITE);

        SetShapesTexture(Stone, Bottom);
        DrawTextureRec(Stone, Bottom, {100,100}, WHITE);

        SetShapesTexture(Stone, LSide);
        DrawTextureRec(Stone, LSide, {100,100}, WHITE);
        
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

    int GridDrawMap(){
        for (y = 0; y <= height; ++y){
            for (x = 0; x <= length; ++x){

                Rectangle rect = {(0 + (80 * x)), (20 + (80 * y)), 80, 80};

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

    int ClassDrawMap(){
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

    int CheckCollisionTop(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.Top))){
            //fprintf(stderr,"Collision\n");
            DrawRectangleRec(player->frameRec, RED);
            DrawRectangleRec(player->hitBox, GREEN);

            if(player->hitBox.y < (startingRoom.Top.height + startingRoom.Top.y) && player ->hitBox.x  > startingRoom.Top.x && player ->hitBox.x < (startingRoom.Top.x + startingRoom.Top.width)){
                
                player->position.y = startingRoom.Top.height + startingRoom.Top.y - 11;
                fprintf(stderr,"Up Collision\n");
            }
        }
    }

    int CheckCollisionBottom(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.Bottom))){
            //fprintf(stderr,"Collision\n");
            DrawRectangleRec(player->frameRec, RED);
            DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if((player->hitBox.y + player->hitBox.height) > (startingRoom.Bottom.y)){
                fprintf(stderr, "Bottom Collision");
                player->position.y  = (startingRoom.Bottom.y - player->hitBox.height - 11);
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    int CheckCollisionLeft(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.LSide))){
            //fprintf(stderr,"Collision\n");
            DrawRectangleRec(player->frameRec, RED);
            DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if((player->hitBox.x  < (startingRoom.LSide.x + startingRoom.LSide.width))){
                fprintf(stderr, "Left Collision");
                player->position.x  = (startingRoom.LSide.x + 70);
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    


    int CheckCollisionRight(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.RSide))){
            //fprintf(stderr,"Collision\n");
            DrawRectangleRec(player->frameRec, RED);
            DrawRectangleRec(player->hitBox, GREEN);

            // && player ->hitBox.x  > startingRoom.Bottom.x && player ->hitBox.x < (startingRoom.Bottom.x + startingRoom.Top.width

            if(((player->hitBox.x + player->hitBox.width) > startingRoom.RSide.x)){
                fprintf(stderr, "Left Collision");
                player->position.x  = (startingRoom.RSide.x - 55);
               // fprintf(stderr,"Up Collision\n");
            }
        }
    }

    int CheckCollision(Player *player){
        CheckCollisionTop(player);
        CheckCollisionBottom(player);
        CheckCollisionLeft(player);
        CheckCollisionRight(player);
    }

};


