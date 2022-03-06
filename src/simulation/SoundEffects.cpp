#include "raylib.h"

class SoundEffects{
    public: 
        void updateSoundEffects(float delta){
            if (!IsSoundPlaying(footsteps)){
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
            else{
                if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN)){
                    StopSound(footsteps);
                }
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