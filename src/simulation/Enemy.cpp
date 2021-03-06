#include <iostream>
#include <stdio.h>
#include "raylib.h"

class Enemy
{
    private:

    public:
        static constexpr float framesSpeed = 8.0f;
        float speed = 1.0f;
        int id;
        Vector2 prevPosition;
        Vector2 position;
        int framesCounter;
        int currentFrame;
        Rectangle hitBox;
        int enemyHealth;
        int currentHealthFrame;
        int healthFramesCounter;
        int enemyType; 
        std::string name;
        int enemyDamage;

        void decrementHealth(){
            enemyHealth--;
        }

        Enemy(int identifyier, std::string enemyName): id(identifyier), name(enemyName), position({1400, 540}), prevPosition({0, 0}), framesCounter(0), currentFrame(0), currentHealthFrame(0), enemyHealth(100), healthFramesCounter(0), enemyDamage(){} 

        Enemy(Vector2 Position, int EnemyDamage, int EnemyHealth, int EnemyType){
            position = Position;
            enemyDamage = EnemyDamage;
            enemyHealth = EnemyDamage;
            enemyType = EnemyType;
        }


};