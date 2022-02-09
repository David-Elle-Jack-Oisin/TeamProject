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
#include "TeamProject/src/simulation/Player.cpp"
#include "TeamProject/src/simulation/Enemy.cpp"



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitAudioDevice(); // Initialize audio device
    Music music = LoadMusicStream("bensound-deepblue.MP3");
    music.looping = true;
    float pitch = 1.0f;

    PlayMusicStream(music);

    InitWindow(screenWidth, screenHeight, "Quest for moisture");

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

                player.UpdatePlayer(deltaTime);

                EndMode2D();

                DrawText("The Game", ((screenWidth / 2) - 220), (screenHeight / 2), 40, GRAY);

                EndDrawing();
            }
        }

    }

    // De-Initialization
    UnloadMusicStream(music); // Unload music stream buffers from RAM

    CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)

    CloseWindow();

    return 0;
}
