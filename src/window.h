#pragma once

#include <SDL2/SDL_render.h>
#include <cstddef>
#include <fractal.h>
#include <SDL2/SDL.h>
class Window{
  public:
    Window(std::size_t width, std::size_t height);
    ~Window();
    void run();

  private:
    void _init();
    std::size_t width_;
    std::size_t height_;
    Fractal fractal_gen;

    // sdl related
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
};
