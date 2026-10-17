#pragma once

#include <complex>
#include <cstddef>
#include <vector>
#include <cstdint>


class Fractal{

  private:
    size_t width_;
    size_t height_;
    unsigned int max_iterations_ = 1000;
    std::vector<uint32_t> colors_;

  public:
    Fractal(size_t width, size_t height);
    Fractal(size_t width, size_t height, unsigned int max_iterations_);
    uint32_t calculate_fractal(std::complex<float>);
    /*
     * Single threaded implementation of the screen rendering for each pixel
     */
    std::vector<uint32_t>& render_screen(float scale);
    uint32_t colorize(size_t iterations);

};
