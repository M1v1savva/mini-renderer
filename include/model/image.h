#pragma once

#include <model/rgb.h>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

struct Image {
    Image() = default;
    void loadBMP(const char* filename);
    void saveBMP(const char* filename);

    size_t width, height;
    std::vector<std::vector<RGB> > pixels;
};

#include "image.tpp"