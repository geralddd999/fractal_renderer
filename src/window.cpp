#include "fractal.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <window.h>
#include <SDL2/SDL.h>
Window::Window(std::size_t width, std::size_t height)
: width_(width), height_(height), fractal_gen(width, height)
{

}

Window::~Window(){
  // free the resources in the inverse order that they were constructed !
  if (texture_){ SDL_DestroyTexture(texture_);}
  if (renderer_){ SDL_DestroyRenderer(renderer_);}
  if (window_){ SDL_DestroyWindow(window_);}

  SDL_Quit();
}

void Window::_init(){
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    throw std::runtime_error("SDL_INIT failed"); // todo : return the error type
  }

  window_ = SDL_CreateWindow("Fractals",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width_, height_, 0);

  if(!window_){
    throw std::runtime_error("SDL_CreateWindow failed");
  }

  renderer_ = SDL_CreateRenderer(window_ ,
    -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer_){
    throw std::runtime_error("SDL_CreateRenderer failed");
  }
  texture_ = SDL_CreateTexture(renderer_,
    SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width_, height_);

  if(!texture_){
    throw std::runtime_error("SDL_CreateTexture failed");
  }
}

void Window::run(){
  bool render_change = true;
  bool running = true;

  while(running){
    SDL_Event e ;
    while (SDL_PollEvent(&e)){
      if( e.type == SDL_QUIT){
        running = false;
      }else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        //make one iteration move
        render_change = true;
      }
    }

    if(render_change){
      auto pixels = fractal_gen.render_screen(1.0);
      SDL_UpdateTexture(texture_,
        nullptr,pixels.data(), width_ * static_cast<int>(sizeof(std::uint32_t)));

      render_change = false;
    }

    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
  }
}
