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

        void updatePosition(float delta){
            
            if (IsKeyDown(KEY_LEFT)) {
                controllableEnemy.prevPosition = controllableEnemy.position; 
                controllableEnemy.position.x -= controllableEnemy.speed*delta;
            }
            if (IsKeyDown(KEY_RIGHT)) { 
                controllableEnemy.prevPosition = controllableEnemy.position; 
                controllableEnemy.position.x += controllableEnemy.speed*delta;        
            }
            if (IsKeyDown(KEY_UP)) {
                controllableEnemy.prevPosition = controllableEnemy.position; 
                controllableEnemy.position.y -= controllableEnemy.speed*delta;        
            }
            if (IsKeyDown(KEY_DOWN)) {
                controllableEnemy.prevPosition = controllableEnemy.position; 
                controllableEnemy.position.y += controllableEnemy.speed*delta;
            }    
        }
        Enemy* getEnemy(){
            return &controllableEnemy;
        }
    private:
        Enemy controllableEnemy;

};