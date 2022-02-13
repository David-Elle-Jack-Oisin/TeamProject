#include <iostream>
#include <stdio.h>
#include "raylib.h"


class Player {
    private:

    public:
        static const int framesSpeed = 8;
        static constexpr float speed = 5.0f;
        int id;
        Vector2 position;
        int framesCounter;
        int currentFrame;
        Player(int identifyier): id(identifyier), position({1000, 1000}), framesCounter(0), currentFrame(0){}   
};
