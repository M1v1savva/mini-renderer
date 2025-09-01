#pragma once

#include <model/rgb.h>

struct ICanvas {
    size_t width, height, depth;
    RGB** map;

    ICanvas(size_t _width, size_t _height, size_t _depth) 
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
                zbuffer[i][j] = numeric_limits<int>::min();
        }
    }

    ~ICanvas() {
        for (size_t i = 0; i < width; i++)
            delete[] map[i];
        delete[] map;

        for (size_t i = 0; i < width; i++)
            delete[] zbuffer[i];
        delete[] zbuffer;
    }

    void update(size_t x, size_t y, size_t z_bound, RGB& color) {
        if (inRange(x, y) && zbuffer[x][y] < z_bound) {
            zbuffer[x][y] = z_bound;
            drawPixel(x, y, color);
        } 
    }

    bool inRange(size_t x, size_t y) const { 
        return (0 <= x && x < width && 0 <= y && y < height); 
    }

    void drawPixel(size_t x, size_t y, RGB color) {
        map[x][y] = color;
    }
}