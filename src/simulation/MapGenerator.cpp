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
    Rectangle RSide {1820, 0, 100, 1080};
    Rectangle LSide {0, 0, 100, 1080};
    Rectangle Bottom {0, 980, 1920, 100};
    Rectangle Center {100, 100, 1720, 880};

    Rectangle rectangles[4] = {Top, RSide, LSide, Bottom};

    int Draw(){
        //Texture2D grASS;
        //grASS = LoadTexture("src-sprites/grASS.png");
        
        //SetShapesTexture(grASS, Top);
       // DrawTextureRec(grASS, Top, {100,100}, GREEN);
        DrawRectangleRec(Top, BLACK);
        DrawRectangleRec(Center, GRAY);
        DrawRectangleRec(RSide, BLACK);
        DrawRectangleRec(LSide, BLACK);
        DrawRectangleRec(Bottom, BLACK);
        
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

    bool collideFromLeft(Player *player){
        return player->prevHitboxPosition.x + player->hitBox.width < startingRoom.Top.x && player->hitboxPosition.x + player->hitBox.width >= startingRoom.Top.x;  
    }

    bool collideFromRight(Player *player){
        return player->prevHitboxPosition.x >= startingRoom.Top.x + startingRoom.Top.width && player->hitboxPosition.x < startingRoom.Top.x +startingRoom.Top.width;
    }   

    bool collideFromTop(Player *player){
        return player->prevHitboxPosition.y + player->hitBox.height < startingRoom.Top.y && player->hitboxPosition.y + player->hitBox.height >= startingRoom.Top.y;  
    }

    bool collideFromBottom(Player *player){
        return player->prevHitboxPosition.y  >= startingRoom.Top.y + startingRoom.Top.height && player->hitboxPosition.y < startingRoom.Top.y + startingRoom.Top.height;  
    }


    int CheckCollision(Player *player){

        if((CheckCollisionRecs(player->hitBox, startingRoom.Top))){
            //fprintf(stderr,"Collision\n");
            DrawRectangleRec(player->frameRec, RED);
            DrawRectangleRec(player->hitBox, GREEN);

            if(collideFromLeft(player)){
                fprintf(stderr,"left Collision\n");
            }

            if(collideFromRight(player)){
                fprintf(stderr,"right Collision\n");
            }

            if(collideFromTop(player)){
                fprintf(stderr,"Top Collision\n");
            }

            if(collideFromBottom(player)){
                fprintf(stderr,"Bottom Collision\n");
            }
            // If hit from bottom
            // if(player->hitBox.y < (startingRoom.Top.height + startingRoom.Top.y) && player ->hitBox.x  > startingRoom.Top.x && player ->hitBox.x < (startingRoom.Top.x + startingRoom.Top.width)){
                
            //     player->position.y = startingRoom.Top.height + startingRoom.Top.y;
            //     fprintf(stderr,"Up Collision\n");
                

            // }
        }
    }

};


