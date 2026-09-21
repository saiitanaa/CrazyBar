#include <iostream>
#include <ostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"

RenderWindowClass::RenderWindowClass(const char* p_title, int p_x, int p_y, int p_w, int p_h, Uint32 flags)
    :window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, p_x, p_y, p_w, p_h, flags);

    if (window == NULL)
    {
        std::cout << "Error Window Failed to init : " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        std::cout << "Error Renderer Failed to init : " << SDL_GetError() << std::endl;
    }
}

RenderWindowClass::~RenderWindowClass()
{
    //SDL_DestroyRenderer(renderer);
    //SDL_Quit();
}

int RenderWindowClass::GetWindowWidth()
{
    //SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);
    return WindowWidth;
}


int RenderWindowClass::GetWindowHeight()
{
    //SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);
    return WindowHeight;
}

int RenderWindowClass::SetWindowWidth(int p_w)
{
    WindowWidth = p_w;
    return WindowWidth;
}

int RenderWindowClass::SetWindowHeight(int p_h)
{
    WindowHeight = p_h;
    return WindowHeight;
}