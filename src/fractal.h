#pragma once

#include "ThreadPool.h"
#include <complex>
#include <cstddef>
#include <vector>
#include <cstdint>

struct FractalOrigin{
  std::complex<float> origin;
  float scale;
};

/*
 *
 */
class Fractal{
  private:
    size_t width_;
    size_t height_;
    unsigned int max_iterations_ = 1000;
    std::vector<uint32_t> colors_;
    FractalOrigin current_origin_ = FractalOrigin{std::complex<float>(0.0, 0.0), 0.0023};

    //Rendering backend
    ThreadPool pool_ = ThreadPool();

  public:
    Fractal(size_t width, size_t height);
    Fractal(size_t width, size_t height, unsigned int max_iterations_);
    uint32_t calculate_fractal(std::complex<float>);
    /*
     * Single threaded implementation of the screen rendering for each pixel
     */
    std::vector<uint32_t>& render_screen(FractalOrigin origin);

    /*
     * Multi-threaded implementation of the screen rendering for each pixel;
     */
    std::vector<uint32_t>& render_screen(FractalOrigin origin, bool multithreaded);

    uint32_t colorize(size_t iterations);
    void map_to_point(int32_t mouse_x, int32_t mouse_y, FractalOrigin& P);
    std::complex<float> to_coords(size_t px, size_t py);

};
