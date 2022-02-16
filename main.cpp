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
#ifndef _ENEMY_H
#define _ENEMY_H
    #include "src/simulation/Enemy.cpp"
#endif
#ifndef _ENEMY_RENDERER_H
#define _ENEMY_RENDERER_H
    #include "src/simulation/EnemyRenderer.cpp"
#endif
#ifndef _ENEMY_CONT_H
#define _ENEMY_CONT_H
    #include "src/simulation/EnemyController.cpp"
#endif
#ifndef _CLIENT_H
#define _CLIENT_H
    #include "src/network/client.cpp"
#endif
#ifndef _MAP_H
#define _MAP_H
    #include "src/simulation/MapGenerator.cpp"
#endif


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    int x;
    int y;
    int z;

    int height = 10;
    int length = 20;

    gameClient client;
    PlayersRenderer playersRender;
    std::thread clientThread([&client, &playersRender](){
        client.startClient(&playersRender);
    });

    EnemyRenderer enemyRender;
    MapGenerator map;

    InitAudioDevice(); // Initialize audio device

    Camera2D camera = { 0 };
    PlayerController playerController;
    Player *ptrPlayer;
    ptrPlayer = playerController.getPlayer();
    playersRender.addNewPlayer(ptrPlayer);

    EnemyController enemyController;
    Enemy *ptrEnemy;
    ptrEnemy = enemyController.getEnemy();
    enemyRender.addNewEnemy(ptrEnemy);


    camera.target = ptrPlayer->position;
    camera.offset = Vector2{ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitWindow(screenWidth, screenHeight, "Quest for moisture");
    playersRender.loadTexture();
    Music music = LoadMusicStream("bensound-jazzyfrenchy.mp3");
    music.looping = true;
    float pitch = 1.0f;

    int framesCounter = 0;

    SetTargetFPS(60);  

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(music);
        PlayMusicStream(music);
        
        float deltaTime = GetFrameTime();

            ClearBackground(RAYWHITE);

            DrawText("Press enter to start", ((screenWidth / 2) - 220), (screenHeight / 2), 40, GRAY);
            EndDrawing();

        if (IsKeyPressed(KEY_ENTER)){
            while(!WindowShouldClose()){
                UpdateMusicStream(music);
                PlayMusicStream(music);

                float deltaTime = GetFrameTime();


                BeginDrawing();

                ClearBackground(RAYWHITE);

                map.DrawMap();

                BeginMode2D(camera);

                playerController.updatePosition(deltaTime);
                playersRender.renderPlayers();
                client.sendPos(ptrPlayer->position);

                

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
