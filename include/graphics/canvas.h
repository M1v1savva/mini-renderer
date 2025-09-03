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

    Canvas(size_t _width, size_t _height, size_t _depth) 
        : 
        width(_width), height(_height), depth(_depth) {
        
        if (width <= 0 || height <= 0)
            new std::runtime_error("Graphics canvas width and height must be positive.");
        map = new RGB*[width];
        for (size_t i = 0; i < width; i++)
            map[i] = new RGB[height];

        zbuffer = new int*[width];
        for (size_t i = 0; i < width; i++) {
            zbuffer[i] = new int[height];
            for (size_t j = 0; j < height; j++)
                zbuffer[i][j] = std::numeric_limits<int>::min();
        }
    }

    ~Canvas() {
        for (size_t i = 0; i < width; i++)
            delete[] map[i];
        delete[] map;

        for (size_t i = 0; i < width; i++)
            delete[] zbuffer[i];
        delete[] zbuffer;
    }

    size_t get_width() const { return width; }
    size_t get_height() const { return height; }
    size_t get_depth() const { return depth; }
    RGB** get_pixelmap() const { return map; }

    void update(size_t x, size_t y, int z_bound, const RGB& color) {
        if (in_range(x, y) && zbuffer[x][y] < z_bound) {
            zbuffer[x][y] = z_bound;
            draw_pixel(x, y, color);
        } 
    }

    bool in_range(size_t x, size_t y) const { 
        return (0 <= x && x < width && 0 <= y && y < height); 
    }

    void draw_pixel(size_t x, size_t y, const RGB& color) {
        map[x][y] = color;
    }
};