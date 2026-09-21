#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


class LoadRessource
{
private:
    // Sprite
    SDL_Texture* Box;
    SDL_Texture* Tray;
    SDL_Texture* BG;
    // Font
    TTF_Font* ScoreGUIFont = TTF_OpenFont("res/font/Oswald_Bold.ttf", 70);
    TTF_Font* ScoreAnimation = TTF_OpenFont("res/font/Oswald_Bold.ttf", 16);
    //Sound
    Mix_Chunk* BonusSound = Mix_LoadWAV("res/sound/bonus.mp3");
    Mix_Chunk* BoxSound1 = Mix_LoadWAV("res/sound/BoxSound1.mp3");
    Mix_Chunk* BoxSound2 = Mix_LoadWAV("res/sound/BoxSound2.mp3");
    Mix_Chunk* BoxSound3 = Mix_LoadWAV("res/sound/BoxSound3.mp3");
    Mix_Chunk* BoxSound4 = Mix_LoadWAV("res/sound/BoxSound4.mp3");
    Mix_Chunk* BoxSound5 = Mix_LoadWAV("res/sound/BoxSound5.mp3");
    Mix_Chunk* LevelUpSound = Mix_LoadWAV("res/sound/level_up.mp3");
public:
    LoadRessource(SDL_Renderer* renderer);
    ~LoadRessource();

    SDL_Texture* GetSprite(std::string TextureName);
    TTF_Font* GetFont(std::string FontName);
    Mix_Chunk* GetSound(std::string SoundName);
};