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
        bool exitButtonAction = false;
    
    public:

    // MainMenu mainmenu;
    void gameOver() {
        Rectangle replayButtonBounds = { GetScreenWidth()/2.0f - 400/2.0f, GetScreenHeight()/2.0f + 150, 400, 100 };

        while(!exitWindow) {
            // EXIT CONDITIONS
            if (exitButtonAction || WindowShouldClose()) {exitWindow = true;}
            mousePoint = GetMousePosition();
        
            // DECTECT buttons clicked
            if (CheckCollisionPointRec(mousePoint, replayButtonBounds))
            {
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)||IsKeyDown(KEY_ENTER)) {exitButtonAction = true;}  //button to go back to main menu
            }
            BeginDrawing();
                ClearBackground(WHITE);
                DrawText("You Died!", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, RED);
                DrawRectangleRec(replayButtonBounds, BLUE);
                DrawText("Main Menu", (GetScreenWidth()/2.0f - 400/2.25f + 50), (GetScreenHeight()/2.0f + 180), 40, BLACK);
 
            EndDrawing();
        }

    }
};
