#include "raylib.h"
#ifndef _PLAYER_H
#define _PLAYER_H
    #include "Player.cpp"
#endif
#ifndef _PLAYER_RENDERER_H
#define _PLAYER_RENDERER_H
    #include "PlayersRenderer.cpp"
#endif


class PlayerController{
    public:
        PlayerController() : controllablePlayer(30){}

        void updatePosition(float delta){
            
            if (IsKeyDown(KEY_LEFT)) {
                controllablePlayer.prevHitboxPosition = controllablePlayer.hitboxPosition;
                controllablePlayer.hitboxPosition.x -= controllablePlayer.speed*delta;
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.x -= controllablePlayer.speed*delta;
                controllablePlayer.positionOffset.x -= controllablePlayer.speed*delta;
            }
            if (IsKeyDown(KEY_RIGHT)) { 
                controllablePlayer.prevHitboxPosition = controllablePlayer.hitboxPosition;
                controllablePlayer.hitboxPosition.x += controllablePlayer.speed*delta;
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.x += controllablePlayer.speed*delta;
                controllablePlayer.positionOffset.x += controllablePlayer.speed*delta;        
            }
            if (IsKeyDown(KEY_UP)) {
                controllablePlayer.prevHitboxPosition = controllablePlayer.hitboxPosition;
                controllablePlayer.hitboxPosition.y -= controllablePlayer.speed*delta;
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.y -= controllablePlayer.speed*delta;   
                controllablePlayer.positionOffset.y -= controllablePlayer.speed*delta;      
            }
            if (IsKeyDown(KEY_DOWN)) {
                controllablePlayer.prevHitboxPosition = controllablePlayer.hitboxPosition;
                controllablePlayer.hitboxPosition.y += controllablePlayer.speed*delta;
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.y += controllablePlayer.speed*delta;
                controllablePlayer.positionOffset.y += controllablePlayer.speed*delta;
            }
            controllablePlayer.hitBox.x = (controllablePlayer.position.x + 30);
            controllablePlayer.hitBox.y = (controllablePlayer.position.y + 12);
        }
        Player* getPlayer(){
            return &controllablePlayer;
        }
    private:
        Player controllablePlayer;

};