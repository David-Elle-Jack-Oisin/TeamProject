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
        static constexpr float speed = 50.0f;
        Vector2 position;
        float directionX;
        float directionY;
        Rectangle hitBox;
};