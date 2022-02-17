#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#ifndef _ENEMY_H
#define _ENEMY_H
    #include "Enemy.cpp"
#endif

class EnemyRenderer{
    public:
        void addNewEnemy(Enemy* Enemy){
            EnemyList.push_back(Enemy);
            id++;
        }
        void renderEnemy(){
            std::list<Enemy*>::iterator iter;
            for (iter = EnemyList.begin(); iter != EnemyList.end(); ++iter){
                renderEnemy(*iter);
            }
        }
        void loadTexture(){
            Slime = LoadTexture("LSlime.png");
            frameRec = { 0.0f, 0.0f, (float)Slime.width/2, (float)Slime.height};
        }
        void updateSecondEnemy(float x, float y){
            EnemyList.back()->prevPosition = EnemyList.back()->position; 
            EnemyList.back()->position.x = x;
            EnemyList.back()->position.y = y;
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
                        Enemy->currentFrame++;     //currentFrame is the current image of Slime from the spritesheet
                        //0-7 is right-facing, 7-14 is left-facing

                        if (Enemy->currentFrame > 2) {Enemy->currentFrame = 1;}

                        //This hides the other frames in the animation
                        //So that the walking looks natural

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

            }
            // DOWN CHECK 
            if (Enemy->prevPosition.y - Enemy->position.y < 0){
                Enemy->prevPosition = Enemy->position;
                // ANIMATION CODE


            }
    
        }
};