#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include <raymath.h>
#ifndef _ENEMY_H
#define _ENEMY_H
    #include "Enemy.cpp"
#endif
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif

class EnemyRenderer{
    public:
        void addNewEnemy(Enemy* Enemy){
            EnemyList.push_back(Enemy);
            Enemy->enemyDamage = 1;
            Enemy->enemyHealth = 5;
        }
        void renderEnemy(){
            std::list<Enemy*>::iterator iter;
            for (iter = EnemyList.begin(); iter != EnemyList.end(); ++iter){
                renderEnemy(*iter);
            }
        }
        void loadTexture(){
            Slime = LoadTexture("src-sprites/LSlime.png");
            frameRec = { 0.0f, 0.0f, (float)Slime.width/2, (float)Slime.height};
        }
        void EnemyAi (Player* player){
            Vector2 nextPosition = Vector2MoveTowards(EnemyList.back()->position, player->position, 2.0);
            EnemyList.back()->prevPosition = EnemyList.back()->position; 
            EnemyList.back()->position = nextPosition;
        }
    private:
        std::list<Enemy*>EnemyList;
        int id;
        Texture2D Slime;
        Rectangle frameRec;
       
        void renderEnemy(Enemy* Enemy){
            DrawTextureRec(Slime, frameRec, Enemy->position, WHITE);
            // LEFT CHECK
            if (Enemy->prevPosition.x - Enemy->position.x > 0){
                Enemy->prevPosition = Enemy->position;

                // ANIMATION CODE

                Enemy->framesCounter++;


                if (Enemy->framesCounter >= (60/Enemy->framesSpeed))
                    {
                        Enemy->framesCounter = 0;
                        Enemy->currentFrame++;

                        if (Enemy->currentFrame > 2) {Enemy->currentFrame = 1;}

                        frameRec.x = (float)Enemy->currentFrame*(float)Slime.width/2;
            } 

            }
            // RIGHT CHECK 
            if (Enemy->prevPosition.x - Enemy->position.x < 0){
                Enemy->prevPosition = Enemy->position;
                // ANIMATION CODE

                Enemy->framesCounter++;


                if (Enemy->framesCounter >= (60/Enemy->framesSpeed))
                    {
                        Enemy->framesCounter = 0;
                        Enemy->currentFrame++;     //currentFrame is the current image of Slime from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing

                        if (Enemy->currentFrame > 2) {Enemy->currentFrame = 1;}

                        //This hides the other frames in the animation
                        //So that the walking looks natural

                        frameRec.x = (float)Enemy->currentFrame*(float)Slime.width/2;
            } 
            }
            // UP CHECK
            if (Enemy->prevPosition.y - Enemy->position.y > 0){
                Enemy->prevPosition = Enemy->position;
                // ANIMATION CODE
                Enemy->framesCounter++;

            }
            // DOWN CHECK 
            if (Enemy->prevPosition.y - Enemy->position.y < 0){
                Enemy->prevPosition = Enemy->position;
                // ANIMATION CODE
                Enemy->framesCounter++;

            }
    
        }
};