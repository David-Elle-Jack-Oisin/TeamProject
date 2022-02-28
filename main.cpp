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
#include "src/simulation/menus/MainMenu.cpp"
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
#ifndef _SERVER_H
#define _SERVER_H
    #include "src/network/server.cpp"
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
    std::thread clientThread;
    // Player Must Be Created And Added To Renderer Before The Thread
    // This Is So We Don't Have To Wait For A Time Out To Start The Game
    PlayersRenderer playersRender;
    PlayerController playerController;
    Player *ptrPlayer;
    ptrPlayer = playerController.getPlayer();
    playersRender.addNewPlayer(ptrPlayer);

    
    MainMenu mainMenu;
    EnemyRenderer enemyRender;
    MapGenerator terrain;

    InitAudioDevice(); // Initialize audio device

    Camera2D camera = { 0 };

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
    enemyRender.loadTexture();

    Music MenuMusic = LoadMusicStream("src-audio/bensound-theelevatorbossanova.mp3");
    MenuMusic.looping = true;

    Music music = LoadMusicStream("src-audio/bensound-jazzyfrenchy.mp3");
    music.looping = true;

    float pitch = 1.0f;

    int framesCounter = 0;

    SetTargetFPS(60);  

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(MenuMusic);
        PlayMusicStream(MenuMusic);
        mainMenu.runMainMenu();

        if (mainMenu.isMainMenuFinished()){
            if (!mainMenu.isSinglePlayer()){
                clientThread = std::thread([&client, &playersRender](){
                    client.startClient(&playersRender);
                });
                int connectFrame = 0;
                while(!client.checkConnected() && !WindowShouldClose()){
                    BeginDrawing();
                        ClearBackground(RAYWHITE);
                        DrawText("Connecting", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, BLACK);
                        if (connectFrame > 0) DrawText(".", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 300), 80, BLACK);
                        if (connectFrame > 1) DrawText(".", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 200), 80, BLACK);
                        if (connectFrame > 2) DrawText(".", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 200), 80, BLACK);
                    EndDrawing();
                    connectFrame++;
                    if (connectFrame == 4){
                        connectFrame = 0;
                    }
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    if (client.checkTimeOut()){
                        while(!WindowShouldClose()){
                            BeginDrawing();
                                ClearBackground(RAYWHITE);
                                DrawText("Failed To Connect", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, BLACK);
                            EndDrawing();
                        }
                        break;
                    }
                }
                if (!client.checkConnected()){
                    mainMenu.clearOptions();
                    client.turnOff();
                    clientThread.join();
                }
            }
        }
        if (mainMenu.isMainMenuFinished()){
            while(!WindowShouldClose()){
                UpdateMusicStream(music);
                PlayMusicStream(music);

                float deltaTime = GetFrameTime();

                BeginDrawing();

                ClearBackground(RAYWHITE);

                terrain.DrawMap();

                BeginMode2D(camera);

                playerController.updatePosition(deltaTime);
                playersRender.renderPlayers();
                if (!mainMenu.isSinglePlayer()){
                    client.sendPlayerInfo(ptrPlayer->id, ptrPlayer->position, ptrPlayer->playerHealth);
                }
                
                enemyController.updatePosition(deltaTime);
                enemyRender.renderEnemy(); 

                EndMode2D();

                EndDrawing();
                                
            }
            if (!mainMenu.isSinglePlayer()){
                client.turnOff();
                clientThread.join();
            }
            mainMenu.clearOptions();
        }
    }
    

    // De-Initialization
    UnloadMusicStream(music); // Unload music stream buffers from RAM
    UnloadMusicStream(MenuMusic); // Unload music stream buffers from RAM
    CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)

    CloseWindow();

    return 0;
}
