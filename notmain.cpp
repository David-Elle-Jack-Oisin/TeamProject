/*******************************************************************************************
*
*   raylib Quest for moisture MVP Version 1
*
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
* 
*   Press enter to start
*   Player can move around the map
*   Enemy spawns after certain time period 
*   Player/Enemy can damage each other
*   Player/Enemy can kill each other
*   If Player dies then the game is over 
*   Multiplayer Connectivity
*
*
********************************************************************************************/

#include <iostream>
#include <stdio.h>
#include "raylib.h"
#include "src/simulation/Player.cpp"



// void UpdatePlayer(Player player, float delta);     


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;



    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = Vector2{ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Quest for moisture");

    int framesCounter = 0;

    SetTargetFPS(60);  

       


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        float deltaTime = GetFrameTime();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Press enter to start", ((screenWidth / 2) - 220), (screenHeight / 2), 40, GRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)){
            while(!WindowShouldClose()){

                float deltaTime = GetFrameTime();

                BeginDrawing();

                ClearBackground(RAYWHITE);

                BeginMode2D(camera);

                player.UpdatePlayer(deltaTime);
                
                EndMode2D();

                DrawText("The Game", ((screenWidth / 2) - 220), (screenHeight / 2), 40, GRAY);

                EndDrawing();
            }
        }

    }

    // De-Initialization

    CloseWindow();

    return 0;
}


