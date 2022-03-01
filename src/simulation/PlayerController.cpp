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
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.x -= controllablePlayer.speed*delta;
            }
            if (IsKeyDown(KEY_RIGHT)) { 
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.x += controllablePlayer.speed*delta;        
            }
            if (IsKeyDown(KEY_UP)) {
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.y -= controllablePlayer.speed*delta;        
            }
            if (IsKeyDown(KEY_DOWN)) {
                controllablePlayer.prevPosition = controllablePlayer.position; 
                controllablePlayer.position.y += controllablePlayer.speed*delta;
            }
            controllablePlayer.hitBox.x = controllablePlayer.position.x;
            controllablePlayer.hitBox.y = controllablePlayer.position.y;
        }
        Player* getPlayer(){
            return &controllablePlayer;
        }
    private:
        Player controllablePlayer;

};