#include "raylib.h"

class SoundEffects{
    public:
        Sound footsteps = LoadSound("src-audio/footsteps.wav");  
        void updateSoundEffects(float delta){
            if (IsKeyPressed(KEY_LEFT)) {
                PlaySound(footsteps);
            }
            else if (IsKeyPressed(KEY_RIGHT)) { 
                PlaySound(footsteps);   
            }
            else if (IsKeyPressed(KEY_UP)) {
                PlaySound(footsteps);    
            }
            else if (IsKeyPressed(KEY_DOWN)) {
                PlaySound(footsteps);
            }
        }
};