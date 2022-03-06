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
#include "src/simulation/SoundEffects.cpp"
#include "src/simulation/menus/GameWon.cpp"
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
#ifndef _GAME_OVER_H
#define _GAME_OVER_H
    #include "src/simulation/menus/GameOver.cpp"
#endif
#ifndef _BULLET_RENDERER_H
#define _BULLET_RENDERER_H
    #include "src/simulation/BulletRenderer.cpp"
#endif
#include <map>


const int screenWidth = 1000;
const int screenHeight = 1000;

void UpdateCameraCenter(Camera2D *camera, Player *player);

// Enemy* createEnemy(){
//             Enemy enemy = Enemy({1000, 1000}, 20, 20, 1);
//             return &enemy;
//         }

int main(void)
{
    // Initializationcamera
    //--------------------------------------------------------------------------------------

    int score;

    gameClient client;
    std::thread clientThread;
    // MENUS 
    SoundEffects soundEffects;
    MainMenu mainMenu;
    GameOver gameover;
    GameWon gamewon;
    // Renderers
    EnemyRenderer enemyRender;
    BulletRenderer bulletRenderer;
    MapGenerator terrain;
    
    InitAudioDevice(); // Initialize audio device
    std::map<int, Player*>playerMap;
    Camera2D camera = { 0 };

    camera.target = {1000,1000};
    camera.offset = Vector2{ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    // enemys 
    Enemy slime(0, "Faithful Slime");
    Enemy skelly(1, "Undead Giant Autar");
    Enemy spoopy(2, "Flame Lord Thalia");
    Enemy* currentEnemy;
    currentEnemy = &slime;
    enemyRender.addNewEnemy(&slime);
    InitWindow(screenWidth, screenHeight, "The Dungeon's Dozen");
    PlayersRenderer playersRender(playerMap);
    playersRender.loadTexture();
    mainMenu.loadTexture();
    enemyRender.loadTexture();
    terrain.loadTextures();
    gameover.loadTexture();
    bulletRenderer.loadTexture();

    Music MenuMusic = LoadMusicStream("src-audio/MenuMusic.mp3");
    MenuMusic.looping = true;
    soundEffects.loadSounds();
    Music music = LoadMusicStream("src-audio/gameMusic.mp3");
    music.looping = true; 

    bool dead = false;
    bool won = false; 

    SetTargetFPS(60);  
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        UpdateMusicStream(MenuMusic);
        PlayMusicStream(MenuMusic);
        mainMenu.runMainMenu();
        dead = false;
        won = false;
        PlayerController playerController;
        Player *ptrPlayer;
        ptrPlayer = playerController.getPlayer();
        playersRender.addNewPlayer(ptrPlayer);
        
    

        if (mainMenu.isMainMenuFinished()){
            if (!mainMenu.isSinglePlayer()){
                clientThread = std::thread([&client, &playersRender,&enemyRender,&bulletRenderer](){
                    client.startClient(&playersRender, &enemyRender, &bulletRenderer);
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
            else{
                slime.enemyHealth = 100;
                slime.position = {1400, 540};
            }
        }
        if (mainMenu.isMainMenuFinished()){
            dead = false;
            won = false;
            while(!WindowShouldClose() & !dead & !won){
                UpdateMusicStream(music);
                PlayMusicStream(music);

                float deltaTime = GetFrameTime();

                UpdateCameraCenter(&camera, playerController.getPlayer());

                BeginDrawing();

                ClearBackground(BLACK);
                
                    
                    BeginMode2D(camera);
                        
                        terrain.ClassDrawMap();
                        terrain.CheckCollision(ptrPlayer);
                        playerController.updatePosition(deltaTime);
                        soundEffects.updateSoundEffects(deltaTime);
                        playersRender.renderPlayers();


                        // Checks to see if no ememies are present
                        if (!enemyRender.activeEnemy()){
                            score++;
                            fprintf(stderr,"%i", score);
                            if (score % 3 == 0){
                            slime.enemyHealth = 100;
                            slime.position = {1400, 540};
                            enemyRender.addNewEnemy(&slime);
                            currentEnemy = &slime;
                            }

                            if (score % 3 == 1){
                            skelly.enemyHealth = 100;
                            skelly.position = {1400, 540};
                            enemyRender.addNewEnemy(&skelly);
                            currentEnemy = &skelly;
                            }

                            if (score % 3 == 2){
                            skelly.enemyHealth = 100;
                            skelly.position = {1400, 540};
                            enemyRender.addNewEnemy(&spoopy);
                            currentEnemy = &spoopy;
                            }



                        }
                        if (!mainMenu.isSinglePlayer()){
                            client.sendPlayerInfo(ptrPlayer->id, ptrPlayer->position, ptrPlayer->playerHealth);
                            client.sendEnemyInfo(1, slime.position, slime.enemyHealth);
                        }
                        if (ptrPlayer->playerHealth <= 0) {
                            if (!mainMenu.isSinglePlayer()){
                                client.sendDeathPacket(ptrPlayer->id);
                            }
                            dead = true;
                        }
                        if (slime.enemyHealth <= 0){
                            enemyRender.removeEnemy(&slime);
                        }
                        if (skelly.enemyHealth <= 0){
                            enemyRender.removeEnemy(&skelly);
                        }
                        if (spoopy.enemyHealth <= 0){
                            enemyRender.removeEnemy(&spoopy);
                        }
                        enemyRender.renderEnemy();
                        enemyRender.findClosestPlayer(playerMap);
                        if (IsKeyPressed(KEY_SPACE)){
                            Bullet bullet;
                            bullet.position = ptrPlayer->position;
                            bullet.hitBox = { ptrPlayer->position.x, ptrPlayer->position.y, 20, 20};
                            if (ptrPlayer->playerXDir < -959) ptrPlayer->playerXDir = -1.675537f; 
                            bullet.directionX = -ptrPlayer->playerXDir;
                            bullet.directionY = -ptrPlayer->playerYDir;
                            bulletRenderer.addNewBullet(bullet);
                            if (!mainMenu.isSinglePlayer()){
                                client.sendBullet(bullet.position.x,bullet.position.y, bullet.directionX, bullet.directionY);
                            }
                        }

                    bulletRenderer.renderBullets(currentEnemy);
                        //bulletRenderer.renderBullets(&slime);
                        // if (slime.enemyHealth <= 0){
                        //     won = true;
                        // }
                        
                    
                    EndMode2D();

                    DrawRectangle( 200, 100, currentEnemy->enemyHealth*10, 10, RED);
                    DrawText(currentEnemy->name.c_str(),200, 50, 50, WHITE);
                    
                    
                EndDrawing();
                                
            }
            if (won) gamewon.won();
            if (dead)gameover.gameOver();
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

void UpdateCameraCenter(Camera2D *camera, Player *player){
    camera->offset = Vector2{ screenWidth/2.0f, screenHeight/2.0f};
    camera->target = player->position;
}

// Enemy* createEnemy(){
//             Enemy enemy = Enemy({1000, 1000}, 20, 20, 1);
//             return &enemy;
//         }Enemy* createEnemy(){
//             Enemy enemy = Enemy({1000, 1000}, 20, 20, 1);
//             return &enemy;
//         }Enemy* createEnemy(){
//             Enemy enemy = Enemy({1000, 1000}, 20, 20, 1);
//             return &enemy;
//         }