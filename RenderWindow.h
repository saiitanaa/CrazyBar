#pragma once
#include <SDL2/SDL.h>

class RenderWindowClass
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int WindowWidth, WindowHeight;
    public:

    RenderWindowClass(const char* p_title, int p_x, int p_y, int p_w, int p_h, Uint32 flags);
    ~RenderWindowClass();
    SDL_Window* GetWindow() {return window;}
    SDL_Renderer* GetRenderer() {return renderer;}
    int GetWindowWidth();
    int GetWindowHeight();
    int SetWindowWidth(int p_w);
    int SetWindowHeight(int p_h);
};
