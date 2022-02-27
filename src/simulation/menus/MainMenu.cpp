#include "raylib.h"
#include <string>
#include <thread>

class MainMenu{
    public:
    void runMainMenu(){
        mousePoint = GetMousePosition();
        Rectangle WideRect = { 0, GetScreenHeight()/2.0f - 425, GetScreenWidth(), 125 };
        Rectangle topButtonBounds = { GetScreenWidth()/2.0f - 400/2.0f, GetScreenHeight()/2.0f - 150, 400, 100 };
        Rectangle bottomButtonBounds = { GetScreenWidth()/2.0f - 400/2.0f, GetScreenHeight()/2.0f + 150, 400, 100 };
        // DECTECT buttons clicked
        if (CheckCollisionPointRec(mousePoint, topButtonBounds))
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)||IsKeyDown(KEY_ENTER)) singlePlayerButtonAction = true;
        }
        else if (CheckCollisionPointRec(mousePoint, bottomButtonBounds))
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) muiltiPlayerButtonAction = true;
        }
        // CHECK FOR MUILTIPLAYER PRESSED
        if (muiltiPlayerButtonAction){
            // PAGE LOOP
            while(!exitWindow){
                // EXIT CONDITIONS
                if (hostButtonAction || isAttemptingToJoinServer || WindowShouldClose()) {exitWindow = true;}
                // CHECK IF JOIN PRESSED
                if (joinButtonAction){
                    // VARS FOR INPUT
                    int maxChars = 15;
                    char ip[maxChars + 1] = "\0";
                    int letterCount = 0;
                    int framesCounter = 0;

                    // PAGE LOOP
                    while(!exitWindow2){
                        if (isAttemptingToJoinServer || WindowShouldClose()) exitWindow2 = true;
                        
                        int key = GetCharPressed();
                        // Check if more characters have been pressed on the same frame
                        while (key > 0){
                            if ((key >= 48) && (key <= 57) && (letterCount < maxChars)){
                                // Add Dots
                                if (((letterCount + 1) % 4 == 0) && letterCount > 0){
                                    ip[letterCount] = '.';
                                    ip[letterCount+1] = '\0';
                                    letterCount++;
                                }
                                ip[letterCount] = (char)key;
                                ip[letterCount+1] = '\0';
                                letterCount++;
                            }
                            key = GetCharPressed();  // Check next character in the queue
                        }
                        if (IsKeyDown(KEY_BACKSPACE)){
                            letterCount--;
                            if (letterCount < 0) letterCount = 0;
                            ip[letterCount] = '\0';
                            // allow progessive removal without extreme speed
                            std::this_thread::sleep_for( std::chrono::milliseconds(200));
                        }
                        if (IsKeyDown(KEY_ENTER)){
                            isAttemptingToJoinServer = true; 
                            ipFromInput = std::string(ip);
                        }

                        framesCounter++;
                        
                        // JOIN PAGE DRAW
                        BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DrawRectangleRec(topButtonBounds, LIGHTGRAY);
                            DrawRectangleLines((int)topButtonBounds.x, (int)topButtonBounds.y, (int)topButtonBounds.width, (int)topButtonBounds.height, RED);

                            DrawText(ip, (int)topButtonBounds.x + 5, (int)topButtonBounds.y + 8, 50, MAROON);
                            
                            if (letterCount < maxChars){
                                // Draw blinking underscore char
                                if (((framesCounter/20)%2) == 0) DrawText("_", (int)topButtonBounds.x + 8 + MeasureText(ip, 50), (int)topButtonBounds.y + 19, 50, MAROON);
                            }
                        EndDrawing();
                    }
                    // RESET BOOLS ON WAY OUT
                    joinButtonAction = false;
                    exitWindow2 = false;

                }
                else{
                    // MUILTIPLAYER PAGE DRAW
                    BeginDrawing();
                        ClearBackground(WHITE);
                        DrawRectangleRec(WideRect, BLUE);
                        DrawText("A Quest for Moisture", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, BLACK);
                        DrawRectangleRec(topButtonBounds, BLUE);
                        DrawText("Join Game", (GetScreenWidth()/2.0f - 400/2.0f + 50), (GetScreenHeight()/2.0f - 130), 40, BLACK);
                        DrawRectangleRec(bottomButtonBounds, BLUE);
                        DrawText("Host Game", (GetScreenWidth()/2.0f - 400/2.0f + 50), (GetScreenHeight()/2.0f + 180), 40, BLACK);  
                    EndDrawing();
                }
                // DECTECT BUTTONS PUSHED 
                mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, topButtonBounds))
                {
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) joinButtonAction = true;
                }
                else if (CheckCollisionPointRec(mousePoint, bottomButtonBounds))
                {
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) hostButtonAction = true;
                }
            }
            muiltiPlayerButtonAction = false;
            exitWindow = false;
        }
        else{
            // MAIN MENU PAGE
            BeginDrawing();
                ClearBackground(WHITE);
                DrawRectangleRec(WideRect, BLUE);
                DrawText("A Quest for Moisture", (GetScreenWidth()/2.0f - 425), (GetScreenHeight()/2.0f - 400), 80, BLACK);
                DrawRectangleRec(topButtonBounds, BLUE);
                DrawText("Single Player", (GetScreenWidth()/2.0f - 400/2.25f + 50), (GetScreenHeight()/2.0f - 120), 40, BLACK);
                DrawRectangleRec(bottomButtonBounds, BLUE);
                DrawText("Muilti Player", (GetScreenWidth()/2.0f - 400/2.25f + 50), (GetScreenHeight()/2.0f + 180), 40, BLACK);  
            EndDrawing();
        }
    }
    bool isSinglePlayer(){
        return singlePlayerButtonAction;
        }
    bool isHosting(){
        return hostButtonAction;
    }
    bool isMainMenuFinished(){
        return singlePlayerButtonAction || hostButtonAction || isAttemptingToJoinServer;
    }
    void clearOptions(){
        singlePlayerButtonAction = false;
        muiltiPlayerButtonAction = false;
        joinButtonAction = false;
        hostButtonAction = false;
        isAttemptingToJoinServer = false;
        bool exitWindow = false;
        bool exitWindow2 = false;
    }
    void printBools(){
        fprintf(stderr, "%i %i %i %i %i %i %i\n",singlePlayerButtonAction, muiltiPlayerButtonAction, joinButtonAction,hostButtonAction,isAttemptingToJoinServer, exitWindow, exitWindow2 );
    }
    std::string getIpFromInput(){
        return ipFromInput;
    }
    MainMenu(): singlePlayerButtonAction(false), hostButtonAction(false){}
    private:
        bool singlePlayerButtonAction = false;
        bool muiltiPlayerButtonAction = false;
        bool joinButtonAction = false;
        bool hostButtonAction = false;
        bool isAttemptingToJoinServer = false;
        std::string ipFromInput;
        Rectangle topButtonBounds;
        Vector2 mousePoint = { 0.0f, 0.0f };
        bool exitWindow = false;
        bool exitWindow2 = false;
};