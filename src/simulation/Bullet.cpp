#include <iostream>
#include <stdio.h>
#include "raylib.h"
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif

class Bullet {
    private:
    
    float radius;
    
    int lifeSpan;
    int shotDamage = 1;

    bool active;

    public:
        static constexpr float speed = 5.0f;
        static const int framesSpeed = 8;
        Vector2 position;
        float directionX;
        float directionY;
        Rectangle hitBox;
        int framesCounter;
        int currentFrame;
        bool hit = false;
        // 1 - up 2 - right 3 - down 4 - left
        int textureDir;
};