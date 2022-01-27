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

class Player
{
    public:
        Vector2 position = {1000, 1000};
        Color color = GREEN;
        //int velocity = 0;
};


#define PLAYER_SPD 100.0f

// void UpdatePlayer(Player player, float delta);     

void UpdatePlayer(Player *player, float delta);

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Player player;

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

                //UpdatePlayer(player, deltaTime);

                UpdatePlayer(&player, deltaTime);

                BeginDrawing();

                ClearBackground(RAYWHITE);

                BeginMode2D(camera);

                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40 ,40};
                DrawRectangleRec(playerRect, player.color);

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

// void UpdatePlayer(Player player, float delta)
// {
//     if (IsKeyDown(KEY_LEFT)) player.position.x -= 200.0f * delta;
//     if (IsKeyDown(KEY_RIGHT)) player.position.x += 200.0f * delta;
//     if (IsKeyDown(KEY_UP)) player.position.y -= 200.0f * delta;
//     if (IsKeyDown(KEY_UP)) player.position.y += 200.0f * delta;

//     player.playerBody = { (player.position.x - 20),(player.position.y - 40), 40, 40};
// }

void UpdatePlayer(Player *player, float delta)
{
    if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_SPD*delta;
    if (IsKeyDown(KEY_RIGHT)) player ->position.x += PLAYER_SPD*delta;
    if (IsKeyDown(KEY_UP)) player ->position.y -= PLAYER_SPD*delta;
    if (IsKeyDown(KEY_DOWN)) player -> position.y += PLAYER_SPD*delta;
}
