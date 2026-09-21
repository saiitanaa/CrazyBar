#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "LoadRessource.h"

LoadRessource::LoadRessource(SDL_Renderer* renderer)
{
    // Sprite
    Box = IMG_LoadTexture(renderer, "res/sprite/Box.png");
    if (Box == nullptr)
    {
        std::cout << "Loading Box Texture Error : " << IMG_GetError() << std::endl;
    }
    Tray = IMG_LoadTexture(renderer, "res/sprite/Tray.png");
    if (Tray == nullptr)
    {
        std::cout << "Loading Tray Texture Error : " << IMG_GetError() << std::endl;
    }
    BG = IMG_LoadTexture(renderer, "res/sprite/Game_BG_2.png");
    if (BG == nullptr)
    {
        std::cout << "Loading BG Texture Error : " << IMG_GetError() << std::endl;
    }

    // Font

    if (ScoreGUIFont == nullptr)
    {
        std::cout << "Font loading Error : " << TTF_GetError() << std::endl;
    }

    if (ScoreAnimation == nullptr)
    {
        std::cout << "ScoreAnimation Font loading Error : " << TTF_GetError() << std::endl;
    }

    // Sound

    if (BonusSound == nullptr)
    {
        std::cout << "BonusSound loading Error : " << Mix_GetError() << std::endl;
    }


    if (BoxSound1 == nullptr)
    {
        std::cout << "BoxSound1 loading Error : " << Mix_GetError() << std::endl;
    }


    if (BoxSound2 == nullptr)
    {
        std::cout << "BoxSound2 loading Error : " << Mix_GetError() << std::endl;
    }

    if (BoxSound3 == nullptr)
    {
        std::cout << "BoxSound3 loading Error : " << Mix_GetError() << std::endl;
    }


    if (BoxSound4 == nullptr)
    {
        std::cout << "BoxSound4 loading Error : " << Mix_GetError() << std::endl;
    }


    if (BoxSound5 == nullptr)
    {
        std::cout << "BoxSound5 loading Error : " << Mix_GetError() << std::endl;
    }


    if (LevelUpSound == nullptr)
    {
        std::cout << "LevelUpSound loading Error : " << Mix_GetError() << std::endl;
    }
}

LoadRessource::~LoadRessource()
{
    //TTF_CloseFont(ScoreGUIFont);
    //TTF_CloseFont(ScoreAnimation);
    //TTF_Quit();
}

SDL_Texture* LoadRessource::GetSprite(std::string TextureName)
{
    if (TextureName == "Box")
    {
        return Box;
    }
    else if (TextureName == "Tray")
    {
        return Tray;
    }
    else if (TextureName == "BG")
    {
        return BG;
    }
    else
    {
        std::cout << "GetSprite Error : You must specify a valid sprite texture" << std::endl;
        return nullptr;
    }
}

TTF_Font* LoadRessource::GetFont(std::string FontName)
{
    if (FontName == "ScoreGUIFont")
    {
        return ScoreGUIFont;
    }
    else if (FontName == "ScoreAnimation")
    {
        return ScoreAnimation;
    }
    else
    {
        std::cout << "GetFont Error : You must specify a valid font" << std::endl;
        return nullptr;
    }
}

Mix_Chunk* LoadRessource::GetSound(std::string SoundName)
{
    if (SoundName == "BonusSound")
    {
        return BonusSound;
    }
    else if (SoundName == "BoxSound1")
    {
        return BoxSound1;
    }
    else if (SoundName == "BoxSound2")
    {
        return BoxSound2;
    }
    else if (SoundName == "BoxSound3")
    {
        return BoxSound3;
    }
    else if (SoundName == "BoxSound4")
    {
        return BoxSound4;
    }
    else if (SoundName == "BoxSound5")
    {
        return BoxSound5;
    }
    else if (SoundName == "LevelUpSound")
    {
        return LevelUpSound;
    }
    return nullptr;
}