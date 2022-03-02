#include <iostream>
#include <stdio.h>
#include "raylib.h"

class Enemy
{
    private:

    public:
    static const int framesSpeed = 8;
        static constexpr float speed = 5.0f;
        int id;
        Vector2 prevPosition;
        Vector2 position;
        int framesCounter;
        int currentFrame;
        Rectangle hitBox;
        int enemyHealth;
        int currentHealthFrame;
        int healthFramesCounter; 

        int enemyDamage;

        Enemy(int identifyier): id(identifyier), position({1400, 540}), prevPosition({0, 0}), framesCounter(0), currentFrame(0), currentHealthFrame(0), enemyHealth(), healthFramesCounter(0), enemyDamage(){} 


};