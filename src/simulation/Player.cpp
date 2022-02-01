#include <iostream>
#include <stdio.h>
#include "raylib.h"

class Player
{
    private:
        #define PLAYER_SPD 100.0f
    public:
        Vector2 position = {1000, 1000};
        Color color = GREEN;
        //int velocity = 0;
        void UpdatePlayer(float delta)
        {

            const float grav = 0.60f;       
//            static bool isGrounded = false;

            //if (player not colliding with platform) then:
            this -> position.y += grav;     //Player will constantly move down until colliding with a platform
 
            if (IsKeyDown(KEY_LEFT)) this->position.x -= PLAYER_SPD*delta;
            if (IsKeyDown(KEY_RIGHT)) this ->position.x += PLAYER_SPD*delta;
            if (IsKeyDown(KEY_UP)) this ->position.y -= PLAYER_SPD*delta;
            //if (IsKeyDown(KEY_DOWN)) this-> position.y += PLAYER_SPD*delta;
            Rectangle playerRect = { position.x - 20, position.y - 40, 40 ,40};
            DrawRectangleRec(playerRect, color);
    }
};