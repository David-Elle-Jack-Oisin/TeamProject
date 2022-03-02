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

class BulletRenderer{
    public:
        void checkCreateBullet(Player* player){
            if (IsKeyPressed(KEY_SPACE)){
                Bullet bullet;
                bullet.position = player->position;
                if (player->playerXDir < -959) player->playerXDir = -1.675537f; 
                bullet.directionX = -player->playerXDir;
                bullet.directionY = -player->playerYDir;
                addNewBullet(bullet);
            }
        }
        
        void renderBullets(){
            std::map<int, Bullet>::iterator iter;
            for (iter = bulletMap.begin(); iter != bulletMap.end(); ++iter){
                renderBullet(&iter->second);
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

       
        void renderBullet(Bullet* bullet){
            DrawRectangleV(bullet->position, {20,20}, RED);

            bullet->position.x += bullet->directionX*bullet->speed;
            bullet->position.y += bullet->directionY*bullet->speed;
        }
};