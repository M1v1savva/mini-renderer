#ifndef IMAGE_H
#define IMAGE_H

#include <mini-renderer/model/rgb.h>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace mini_renderer {

class Image {
public:
    size_t width, height;
    std::vector<std::vector<RGB> > pixels;

    Image() = default;
    void loadBMP(const char* filename);
    void saveBMP(const char* filename);
};

} 

#include "image.tpp"

#endif // IMAGE_H
