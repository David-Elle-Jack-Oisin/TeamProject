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
        void checkCreateBullet(Player* player){
            if (IsKeyPressed(KEY_SPACE)){
                Bullet bullet;
                bullet.position = player->position;
                bullet.hitBox = { player->position.x, player->position.y, 20, 20};
                if (player->playerXDir < -959) player->playerXDir = -1.675537f; 
                bullet.directionX = -player->playerXDir;
                bullet.directionY = -player->playerYDir;
                addNewBullet(bullet);
            }
        }
        
        void renderBullets(Enemy* curEnemy){
            std::map<int, Bullet>::iterator iter;
            for (iter = bulletMap.begin(); iter != bulletMap.end(); ++iter){
                renderBullet(&iter->second, curEnemy);
            }
        }

    private:
        std::map<int, Bullet>bulletMap;
        int id = 0;

        void addNewBullet(Bullet bullet){
            bulletMap.insert({id, bullet});
            id++; 
        }
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