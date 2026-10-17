#include "fractal.h"
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

  auto color = (iterations/max_iterations_) * 255;
  return color;
}

std::vector<uint32_t>& Fractal::render_screen(float scale){

  auto to_coords = [&](size_t px, size_t py){
    float re = (px - float(width_)/2)*scale;
    float im = -(py - float(height_)/2)*scale;
    return std::complex<float>(re, im);
  };

  for(size_t h = 0 ; h < height_ ; ++h){
    for(size_t w = 0 ; w < width_ ; ++w){
      size_t idx = h*width_ + w;
      colors_[idx] = calculate_fractal(to_coords(w, h));
    }
  }

  return colors_;
}

uint32_t Fractal::colorize(size_t iterations){

}
