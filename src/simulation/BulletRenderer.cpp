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

    private:
        std::map<int, Bullet>bulletMap;
        int id = 0;

        void removeBullet(int id){
            bulletMap.erase(id);     
        }

       
        void renderBullet(Bullet* bullet, Enemy* curEnemy){
            DrawRectangleRec(bullet->hitBox, RED);
            if (CheckCollisionRecs(bullet->hitBox, curEnemy->hitBox)){
                curEnemy->decrementHealth();
            }
            bullet->position.x += bullet->directionX*bullet->speed;
            bullet->position.y += bullet->directionY*bullet->speed;
            bullet->hitBox.x = bullet->position.x;
            bullet->hitBox.y = bullet->position.y;
        }
};