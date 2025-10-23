#pragma once

#include <model/rgb.h>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include <iostream>

struct Canvas {
    size_t width, height, depth;
    RGB** map;
    int** zbuffer;

    Canvas(size_t _width, size_t _height, size_t _depth);
    ~Canvas();

    size_t get_width() const;
    size_t get_height() const;
    size_t get_depth() const;
    RGB** get_pixelmap() const;

    void update(int x, int y, int z_bound, const RGB& color);
    bool in_range(int x, int y) const;
    void draw_pixel(size_t x, size_t y, const RGB& color);
};