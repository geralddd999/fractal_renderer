#include "fractal.h"
#include "TaskSet.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <complex>

Fractal::Fractal(size_t width, size_t height)
: width_(width), height_(height), colors_(width * height)
{
}

Fractal::Fractal(size_t width, size_t height, unsigned int max_iter)
: width_(width), height_(height), max_iterations_(max_iter), colors_(width * height)
{
}

uint32_t Fractal::calculate_fractal(std::complex<float> c){

  auto z = c;
  auto bounded = [&](std::complex<float> z){
    float mod = powf(z.imag(), 2) + powf(z.real(), 2);
    return mod > 4.0 ? false : true ;
  };
  size_t iterations;
  for(iterations = 1 ; (iterations < max_iterations_) && bounded(z) ; ++iterations){
    //z = z**2 + c;
    float re = (z.real() * z.real()) - (z.imag() * z.imag());
    float im = 2.0 * z.real() *z.imag();
    std::complex<float> z_pow2(re, im);

    z = z_pow2 + c;
  }

  return iterations;
}

std::vector<uint32_t>& Fractal::render_screen(FractalOrigin P){

  current_origin_.origin = P.origin + (current_origin_.origin - P.origin)*(P.scale/current_origin_.scale);
  current_origin_.scale = P.scale;
  auto to_coords = [&](size_t px, size_t py){
    float re = current_origin_.origin.real() + (static_cast<float>(px) - static_cast<float>(width_)/2)*current_origin_.scale;
    float im = current_origin_.origin.imag() -(static_cast<float>(py) - static_cast<float>(height_)/2)*current_origin_.scale;
    return std::complex<float>(re, im);
  };

    for(size_t h = 0 ; h < height_ ; ++h){
    for(size_t w = 0 ; w < width_ ; ++w){
      size_t idx = h*width_ + w;
      auto iter =  calculate_fractal(to_coords(w, h));
      colors_[idx] = colorize(iter);
    }
  }
  return colors_;
}

std::vector<uint32_t>& Fractal::render_screen(FractalOrigin P, bool multithreaded){
  current_origin_.origin = P.origin + (current_origin_.origin - P.origin)*(P.scale/current_origin_.scale);
  current_origin_.scale = P.scale;

  std::size_t num_chunks = height_ / pool_.size();
  int rows_per_chunk = (height_ + num_chunks -1) / num_chunks;

  TaskSet tasks;
  for(size_t i = 0 ; i < num_chunks ; ++i){
    auto y_start = i * rows_per_chunk;
    auto y_end = std::min(y_start + rows_per_chunk, height_);
    pool_.execute_par(tasks, [y_start, y_end,width = width_, &c = colors_, this](){
      for(size_t h = y_start ; h < y_end ; ++h){
          for(size_t w = 0 ; w < width ; ++w){
            size_t idx = h*width + w;
            auto iter =  calculate_fractal(to_coords(w, h));
            c[idx] = colorize(iter);
          }
        }
    });
  }
  tasks.wait();

  return colors_;
}

std::complex<float> Fractal::to_coords(size_t px, size_t py){
  float re = current_origin_.origin.real() + (static_cast<float>(px) - static_cast<float>(width_)/2)*current_origin_.scale;
  float im = current_origin_.origin.imag() -(static_cast<float>(py) - static_cast<float>(height_)/2)*current_origin_.scale;
  return std::complex<float>(re, im);
}


void Fractal::map_to_point(int32_t mouse_x, int32_t mouse_y, FractalOrigin& P){
  float re = current_origin_.origin.real() + (static_cast<float>(mouse_x) - static_cast<float>(width_)/2)*current_origin_.scale;
  float im = current_origin_.origin.imag() -(static_cast<float>(mouse_y) - static_cast<float>(height_)/2)*current_origin_.scale;

  P.origin = std::complex<float>(re,im);
}
uint32_t Fractal::colorize(size_t iterations){
  if(iterations >=max_iterations_){
    return 0xFF000000u;
  }

  float t = std::log1p(iterations)/std::log1p(max_iterations_);
  t = std::clamp(t* 255.0, 0.0 , 255.0);

  //what the hell is the argb8888 rep;
  return 0xFF000000u | (static_cast<std::uint32_t>(t) << 16) | (static_cast<std::uint32_t>(t) << 8) | static_cast<std::uint32_t>(t);
}
