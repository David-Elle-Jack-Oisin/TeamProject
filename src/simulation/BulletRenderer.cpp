#include <map>
#include <algorithm>
#include <iostream>
#include <list>
#include <iterator>
#include "raylib.h"
#ifndef _BULLET_H
#define _BULLET_H
    #include "Bullet.cpp"
#endif
#ifndef _ENEMY_H
#define _ENEMY_H
    #include "src/simulation/Enemy.cpp"
#endif
class BulletRenderer{
    public:
        void addBullet(float posX, float posY, float dirX, float dirY){
            Bullet bullet;
            bullet.position.x = posX;
            bullet.position.y = posY;
            bullet.directionX = dirX;
            bullet.directionY = dirY;
            bullet.hitBox = { bullet.position.x, bullet.position.y, 20, 20};
            addNewBullet(bullet);
        }
        void renderBullets(Enemy* curEnemy){
            std::map<int, Bullet>::iterator iter;
            for (iter = bulletMap.begin(); iter != bulletMap.end(); ++iter){
                renderBullet(&iter->second, curEnemy);
            }
        }

        void addNewBullet(Bullet bullet){
            bulletMap.insert({id, bullet});
            id++; 
        }


        // ===================== Work In Progress =======================
        void loadTexture(){
            rightBullet = LoadTexture("src-sprites/RfireBullet.png");
            frameRec = { 0.0f, 0.0f, (float)rightBullet.width/12, (float)rightBullet.height};

            // Skelly = LoadTexture("src-sprites/Skelly.png");
            // frameRec = { 0.0f, 0.0f, (float)Slime.width/14, (float)Slime.height};
            
        }

    private:
        std::map<int, Bullet>bulletMap;
        int id = 0;
        Texture2D rightBullet;
        Texture2D leftBullet;
        Texture2D upBullet;
        Texture2D downBullet;
        Rectangle frameRec;


        void removeBullet(int id){
            bulletMap.erase(id);     
        }

       
        void renderBullet(Bullet* bullet, Enemy* curEnemy){
            Vector2 bulletPosition {bullet->hitBox.x, bullet->hitBox.y};
            DrawTextureRec(rightBullet, frameRec, bulletPosition, WHITE);
            if (CheckCollisionRecs(bullet->hitBox, curEnemy->hitBox)){
                curEnemy->decrementHealth();
            }



            bullet->framesCounter++;


            if (bullet->framesCounter >= (60/bullet->framesSpeed))
                {
                    bullet->framesCounter = 0;
                    bullet->currentFrame++;

                    if (bullet->currentFrame > 12) {bullet->currentFrame = 1;}

                    frameRec.x = (float)bullet->currentFrame*(float)rightBullet.width/12;
            }


            bullet->position.x += bullet->directionX*bullet->speed;
            bullet->position.y += bullet->directionY*bullet->speed;
            bullet->hitBox.x = bullet->position.x;
            bullet->hitBox.y = bullet->position.y;
        }
};