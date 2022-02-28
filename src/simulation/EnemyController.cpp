#include "raylib.h"
#ifndef _ENEMY_H
#define _ENEMY_H
    #include "Enemy.cpp"
#endif
#ifndef _ENEMY_RENDERER_H
#define _ENEMY_RENDERER_H
    #include "EnemyRenderer.cpp"
#endif

class EnemyController{
    public:
        EnemyController() : controllableEnemy(30){}

        Enemy* getEnemy(){
            return &controllableEnemy;
        }
    private:
        Enemy controllableEnemy;

};