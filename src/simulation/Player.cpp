#include <iostream>
#include <stdio.h>
#include "raylib.h"


class Player {
    private:

    public:
        static const int framesSpeed = 8;
        static constexpr float speed = 100.0f;
        int id;
        Vector2 prevPosition;
        Vector2 position;
        Vector2 prevHitboxPosition;
        Vector2 hitboxPosition;
        int framesCounter;
        int currentFrame;
        Rectangle frameRec;
        Rectangle healthFrameRec;
        Rectangle hitBox;
        Vector2 positionOffset;
        float playerXDir;
        float playerYDir;
        int playerHealth;
        int currentHealthFrame;
        int healthFramesCounter;

        Player(int identifyier): id(identifyier), position({960, 540}), prevPosition({0, 0}),hitboxPosition({960 + 30, 540 + 12}), prevHitboxPosition({0, 0}), framesCounter(0), healthFramesCounter(0), currentFrame(0), currentHealthFrame(0), playerHealth(){}

        void matchIdToServer(int identifyierServer){
            id = identifyierServer;
        }
        void decrementHealth(){
            playerHealth--;
            currentHealthFrame++;
        }
  
};
