#include "raylib.h"

class SoundEffects{
    public: 
        void updateSoundEffects(float delta){
            if (IsKeyDown(KEY_LEFT)) {
                PlaySound(footsteps);
            }
            else if (IsKeyDown(KEY_RIGHT)) { 
                PlaySound(footsteps);   
            }
            else if (IsKeyDown(KEY_UP)) {
                PlaySound(footsteps);    
            }
            else if (IsKeyDown(KEY_DOWN)) {
                PlaySound(footsteps);
            }
        }
        void loadSounds(){
            footsteps = LoadSound("src-audio/footsteps.wav"); 
            squish = LoadSound("src-audio/slime.mp3");
        }
    private:
        Sound footsteps;
        Sound squish;
};