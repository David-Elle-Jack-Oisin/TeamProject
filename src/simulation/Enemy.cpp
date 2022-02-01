#include <iostream>
#include <stdio.h>
#include "raylib.h"

const int screenWidth = 1920;
const int screenHeight = 1080;

class Enemy
{
    private:
    #define ENEMY_SPD 100.0f
    #define delta 0.75f

    public:
        float max_enemies = 10;
        float radius;
        bool active;
        float posx = GetRandomValue(100, screenWidth);
        float posy = GetRandomValue(550, screenHeight);
        Vector2 position = {posx, posy};
        Color color = RED;

        //int velocity = 0;
        void UpdateEnemy()
        {

            this->position.x -= ENEMY_SPD * delta;
            this->position.x += ENEMY_SPD * delta;
            this->position.y -= ENEMY_SPD * delta;
            this->position.y += ENEMY_SPD * delta;
        
            Rectangle enemyRect = {position.x - 30, position.y - 40, 40, 40};
            DrawRectangleRec(enemyRect, color);
            
        }
};