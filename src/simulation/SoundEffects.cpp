#include "raylib.h"

class SoundEffects{
    public: 
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
        void loadSounds(){
            footsteps = LoadSound("src-audio/footsteps.wav"); 
        }
    private:
        Sound footsteps;
};