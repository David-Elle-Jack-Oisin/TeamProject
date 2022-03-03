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
        void addNewEnemy(Enemy* enemy){
            EnemyList.push_back(enemy);
            enemy->enemyDamage = 1;
            enemy->hitBox = { enemy->position.x + 50, enemy->position.y + 50, (float)320/2, (float)140};
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

            // Skelly = LoadTexture("src-sprites/Skelly.png");
            // frameRec = { 0.0f, 0.0f, (float)Slime.width/14, (float)Slime.height};
            
        }
        void findClosestPlayer(std::map<int, Player*> &playerMap){
            std::map<int, Player*>::iterator iter;
            Player* closestPlayer;
            float distance = 1000000.f;
            bool set = false;
            for (iter = playerMap.begin(); iter != playerMap.end(); ++iter){
                if(!colliding){
                    if (CheckCollisionRecs(iter->second->hitBox, EnemyList.back()->hitBox)){
                        iter->second->decrementHealth();
                        damageTimer = 1;
                        colliding = true;
                    }
                }
                else{
                    if (!CheckCollisionRecs(iter->second->hitBox, EnemyList.back()->hitBox)){
                        colliding = false;
                    }
                    if (damageTimer % 20 == 0){
                        iter->second->decrementHealth();
                    }
                    damageTimer++;
                }
                
                float playerDistance = Vector2Distance(iter->second->position, EnemyList.back()->position);
                if (playerDistance < distance){
                    distance = playerDistance;
                    
                    closestPlayer = iter->second;
                    set = true;
                }
            }
            if (set){
                EnemyAi(closestPlayer);
            }
            
        }
        void EnemyAi (Player* player){
            
            Vector2 nextPosition = Vector2MoveTowards(EnemyList.back()->position, player->positionOffset, 1.0);
            EnemyList.back()->prevPosition = EnemyList.back()->position; 
            EnemyList.back()->position = nextPosition;
            EnemyList.back()->hitBox.x = EnemyList.back()->position.x + 50;
            EnemyList.back()->hitBox.y = EnemyList.back()->position.y + 50;
        }
        void setEnemyPosition(float posX, float posY){
            EnemyList.back()->position.x = posX;
            EnemyList.back()->position.y = posY;
        }
    private:
        std::list<Enemy*>EnemyList;
        int id;
        Texture2D Slime;
        Texture2D Skelly;
        Rectangle frameRec;
        bool colliding;
        int damageTimer;
       
        void renderEnemy(Enemy* Enemy){
            DrawTextureRec(Slime, frameRec, Enemy->position, WHITE);
            // DrawTextureRec(Skelly, frameRec, Enemy->position, WHITE);
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