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
#include "src/simulation/PlayerController.cpp"
#ifndef _PLAYER_RENDERER_H
#define _PLAYER_RENDERER_H
    #include "src/simulation/PlayersRenderer.cpp"
#endif
#include "src/simulation/Enemy.cpp"
#ifndef _CLIENT_H
#define _CLIENT_H
    #include "src/network/client.cpp"
#endif


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    gameClient client;
    PlayersRenderer playersRender;
    std::thread clientThread([&client, &playersRender](){
        client.startClient(&playersRender);
    });
    
    Enemy enemy;

    InitAudioDevice(); // Initialize audio device

    Camera2D camera = { 0 };
    PlayerController playerController;
    Player *ptrPlayer;
    ptrPlayer = playerController.getPlayer();
    playersRender.addNewPlayer(ptrPlayer);

    camera.target = ptrPlayer->position;
    camera.offset = Vector2{ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Music music = LoadMusicStream("bensound-deepblue.mp3");
    music.looping = true;
    float pitch = 1.0f;

    PlayMusicStream(music);

    InitWindow(screenWidth, screenHeight, "Quest for moisture");
    playersRender.loadTexture();

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

                playerController.updatePosition(delta);
                playersRender.renderPlayers();
                client.sendPos(ptrPlayer->position);
                
                enemy.UpdateEnemy();

                EndMode2D();

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
