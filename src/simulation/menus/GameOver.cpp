#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include "raylib.h"
#ifndef _PLAYER_RENDERER_H
#define _PLAYER_RENDERER_H
    #include "src/simulation/PlayersRenderer.cpp"
#endif

class GameOver{
    private:
        Vector2 mousePoint = { 0.0f, 0.0f };
        bool exitWindow = false;
        bool hostButtonAction = false;
        bool exitButtonAction = false;
    
    public:

    // MainMenu mainmenu;
    void gameOver() {
        mousePoint = GetMousePosition();
        Rectangle replayButtonBounds = { GetScreenWidth()/2.0f - 400/2.0f, GetScreenHeight()/2.0f + 150, 400, 100 };
        // DECTECT buttons clicked
        if (CheckCollisionPointRec(mousePoint, replayButtonBounds))
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)||IsKeyDown(KEY_ENTER)) {exitButtonAction = true; CloseWindow();}  //button to go back to main menu
        }

        while(!exitWindow) {
            // EXIT CONDITIONS
                if (hostButtonAction || exitButtonAction || WindowShouldClose()) {exitWindow = true; CloseWindow();}
            BeginDrawing();
                ClearBackground(WHITE);
                DrawText("You Died!", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, RED);
                DrawRectangleRec(replayButtonBounds, WHITE);
                DrawText("Quit Game", (GetScreenWidth()/2.0f - 400/2.25f + 50), (GetScreenHeight()/2.0f + 180), 40, BLACK);
 
            EndDrawing();
        }

    }
};
