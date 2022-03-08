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
        int score = 0;
        void addNewEnemy(Enemy* enemy){
            EnemyList.push_back(enemy);
            enemy->enemyDamage = 1;
            enemy->hitBox = { enemy->position.x + 50, enemy->position.y + 50, (float)320/2, (float)140};
        }
        void removeEnemy(Enemy* enemy){
            EnemyList.remove(enemy);
        }
        void renderEnemy(){
            if (EnemyList.size() > 0){
                std::list<Enemy*>::iterator iter;
                for (iter = EnemyList.begin(); iter != EnemyList.end(); ++iter){
                    renderEnemy(*iter);
                }
            }
        }
        void loadTexture(){


            Slime = LoadTexture("src-sprites/LSlime.png");
            frameRecSlime = { 0.0f, 0.0f, (float)Slime.width/2, (float)Slime.height};

            Skelly = LoadTexture("src-sprites/Skelly.png");
            frameRecSkelly = { 0.0f, 0.0f, (float)Skelly.width/14.0f, (float)Skelly.height};

            Skelly = LoadTexture("src-sprites/Skelly.png");
            frameRecSkelly = { 0.0f, 0.0f, (float)Skelly.width/14.0f, (float)Skelly.height};

            Spoopy = LoadTexture("src-sprites/SpoopyUpdated.png");
            frameRecSpoopy = {0.0f, 0.0f, (float)Spoopy.width/22.0f, (float)Spoopy.height};


            
        }
        void findClosestPlayer(std::map<int, Player*> &playerMap){
            if (EnemyList.size() > 0){
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
        }
        void EnemyAi (Player* player){
            
            Vector2 nextPosition = Vector2MoveTowards(EnemyList.back()->position, player->positionOffset, EnemyList.back()->speed);
            EnemyList.back()->prevPosition = EnemyList.back()->position; 
            EnemyList.back()->position = nextPosition;
            EnemyList.back()->hitBox.x = EnemyList.back()->position.x + 50;
            EnemyList.back()->hitBox.y = EnemyList.back()->position.y + 50;
        }
        void setEnemyPosition(float posX, float posY, int health, int scoreNew){
            EnemyList.back()->position.x = posX;
            EnemyList.back()->position.y = posY;
            EnemyList.back()->enemyHealth = health;
            score = scoreNew;
            fprintf(stderr,"ENEMY: EnemyPos and health set\n");
        }

        bool activeEnemy(){
            if (EnemyList.empty()){
                return false;
            }
            return true;
        }

        // void createEnemy(){
        //     Enemy enemy = Enemy({1000, 1000}, 20, 20, 1);
        //     addNewEnemy(&enemy);
        // }

        
    private:
        std::list<Enemy*>EnemyList;
        int id;
        Image image;
        Texture2D Slime;
        Texture2D Skelly;
        Texture2D Spoopy;
        Rectangle frameRecSlime;
        Rectangle frameRecSkelly;
        Rectangle frameRecSpoopy;
        bool colliding;
        int damageTimer;
       
        void renderEnemy(Enemy* Enemy){


            if(Enemy->id == 0){

            DrawTextureRec(Slime, frameRecSlime, Enemy->position, WHITE);
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

                        frameRecSlime.x = (float)Enemy->currentFrame*(float)Slime.width/2;
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

                        frameRecSlime.x = (float)Enemy->currentFrame*(float)Slime.width/2;
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






            if(Enemy->id == 1){

                //DrawTexturePro(Skelly, frameRecSkelly,{ 0.0f, 0.0f, (((float)Skelly.width/14) * 2), (((float)Skelly.height)* 2 )}, Enemy->position, 0,WHITE);
               // DrawTextureRec(Skelly, frameRecSkelly, Enemy->position, WHITE);
               DrawTexturePro(Skelly, frameRecSkelly,{ Enemy->position.x, Enemy->position.y, (((float)Skelly.width/14.0f) * 3.0f), (((float)Skelly.height)* 3.0f )}, {0.0f,0.0f}, 0.0f, WHITE);


                // Left Check
            if (Enemy->prevPosition.x - Enemy->position.x > 0){
                Enemy->prevPosition = Enemy->position;

                // ANIMATION CODE

                Enemy->framesCounter++;


                if (Enemy->framesCounter >= (60.0f/Enemy->framesSpeed))
                    {
                        Enemy->framesCounter = 0;
                        Enemy->currentFrame++;

                        if (Enemy->currentFrame > 13) {Enemy->currentFrame = 8;}

                        frameRecSkelly.x = (float)Enemy->currentFrame*(float)Skelly.width/14.0f;
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

                        if (Enemy->currentFrame > 6) {Enemy->currentFrame = 1;}

                        //This hides the other frames in the animation
                        //So that the walking looks natural

                        frameRecSkelly.x = (float)Enemy->currentFrame*(float)Skelly.width/14.0f;
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

        if (Enemy->id == 2){
                                                                                                 // Multiplying by 2.0f resizes the texture by a factor of 2
            DrawTexturePro(Spoopy, frameRecSpoopy,{ Enemy->position.x, Enemy->position.y, (((float)Spoopy.width/22.0f) * 2.0f), (((float)Spoopy.height)* 2.0f )}, {0.0f,0.0f}, 0.0f, WHITE);

                        // Left Check
            if (Enemy->prevPosition.x - Enemy->position.x > 0){
                Enemy->prevPosition = Enemy->position;

                // ANIMATION CODE

                Enemy->framesCounter++;


                if (Enemy->framesCounter >= (60.0f/Enemy->framesSpeed))
                    {
                        Enemy->framesCounter = 0;
                        Enemy->currentFrame++;

                        if (Enemy->currentFrame > 21) {Enemy->currentFrame = 12;}

                        frameRecSpoopy.x = (float)Enemy->currentFrame*(float)Spoopy.width/22.0f;
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

                        if (Enemy->currentFrame > 10) {Enemy->currentFrame = 1;}

                        //This hides the other frames in the animation
                        //So that the walking looks natural

                        frameRecSpoopy.x = (float)Enemy->currentFrame*(float)Spoopy.width/22.0f;
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

    
        }
};