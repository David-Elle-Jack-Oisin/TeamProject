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
            if(bullet.directionY < 0){ bullet.textureDir = 1;}
            if(bullet.directionY > 0){ bullet.textureDir = 3;}
            if(bullet.directionX > 0){ bullet.textureDir = 2;}
            if(bullet.directionX < 0){ bullet.textureDir = 4;}
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
            leftBullet = LoadTexture("src-sprites/LfireBullet.png");
            upBullet = LoadTexture("src-sprites/UfireBullet.png");
            downBullet = LoadTexture("src-sprites/DfireBullet.png");
            frameRecRight = { 0.0f, 0.0f, (float)rightBullet.width/12, (float)rightBullet.height};
            frameRecLeft = { 0.0f, 0.0f, (float)leftBullet.width/12, (float)leftBullet.height};
            frameRecUp = { 0.0f, 0.0f, (float)upBullet.width, (float)upBullet.height/12};
            frameRecDown = { 0.0f, 0.0f, (float)downBullet.width, (float)downBullet.height/12};

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
        Rectangle frameRecRight;
        Rectangle frameRecLeft;
        Rectangle frameRecUp;
        Rectangle frameRecDown;


        void removeBullet(int id){
            bulletMap.erase(id);     
        }

       
        void renderBullet(Bullet* bullet, Enemy* curEnemy){
            Vector2 bulletPosition {bullet->hitBox.x, bullet->hitBox.y};
            
            if(bullet->directionX > 0){DrawTextureRec(rightBullet, frameRecRight, bulletPosition, WHITE);}
            if(bullet->directionX < 0){DrawTextureRec(leftBullet, frameRecLeft, bulletPosition, WHITE);}
            if(bullet->directionY < 0){DrawTextureRec(upBullet, frameRecUp, bulletPosition, WHITE);}
            if(bullet->directionY > 0){DrawTextureRec(downBullet, frameRecDown, bulletPosition, WHITE);}
            
            
            if (bullet->hit == false && CheckCollisionRecs(bullet->hitBox, curEnemy->hitBox)){
                curEnemy->decrementHealth();
                bullet->hit = true;
            }



            bullet->framesCounter++;


            if (bullet->framesCounter >= (60/bullet->framesSpeed))
                {
                    bullet->framesCounter = 0;
                    bullet->currentFrame++;

                    if (bullet->currentFrame > 12) {bullet->currentFrame = 1;}

                    frameRecRight.x = (float)bullet->currentFrame*(float)rightBullet.width/12;
                    frameRecLeft.x = (float)bullet->currentFrame*(float)leftBullet.width/12;
                    frameRecDown.y = (float)bullet->currentFrame*(float)downBullet.height/12;
                    frameRecUp.y = (float)bullet->currentFrame*(float)upBullet.height/12;
            }


            bullet->position.x += bullet->directionX*bullet->speed;
            bullet->position.y += bullet->directionY*bullet->speed;
            bullet->hitBox.x = bullet->position.x;
            bullet->hitBox.y = bullet->position.y;
        }
};