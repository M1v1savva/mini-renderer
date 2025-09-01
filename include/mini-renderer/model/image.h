#ifndef IMAGE_H
#define IMAGE_H

#include <model/rgb.h>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace mini_renderer {

template<size_t width, size_t height> 
class Image {
public:
    static_assert(width > 0 && height > 0, "Image dimensions must be positive");
	std::vector<std::vector<RGB> > pixels = NULL;

    Image() = default;

    void loadBMP(const char* filename) {
        std::ifstream f(filename, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot open file");

        unsigned char header[54];
        f.read(reinterpret_cast<char*>(header), 54);

        width = *reinterpret_cast<int*>(&header[18]);
        height = *reinterpret_cast<int*>(&header[22]);

        size_t size = 3 * width * height;
        std::vector<unsigned char> data(size);
        f.read(reinterpret_cast<char*>(data.data()), size);

        pixels.resize(height, std::vector<RGB>(width));

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                size_t idx = (i * width + j) * 3;
                pixels[i][j] = RGB(data[idx + 2], data[idx + 1], data[idx]);
            }
        }
    }

    void saveBMP(const char* filename) {
        std::ofstream f(filename, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot open file");

        size_t filesize = 54 + 3 * width * height;
        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

        bmpfileheader[ 2] = (unsigned char)(filesize);
        bmpfileheader[ 3] = (unsigned char)(filesize>>8);
        bmpfileheader[ 4] = (unsigned char)(filesize>>16);
        bmpfileheader[ 5] = (unsigned char)(filesize>>24);

        bmpinfoheader[ 4] = (unsigned char)(width);
        bmpinfoheader[ 5] = (unsigned char)(width>>8);
        bmpinfoheader[ 6] = (unsigned char)(width>>16);
        bmpinfoheader[ 7] = (unsigned char)(width>>24);

        bmpinfoheader[ 8] = (unsigned char)(height);
        bmpinfoheader[ 9] = (unsigned char)(height>>8);
        bmpinfoheader[10] = (unsigned char)(height>>16);
        bmpinfoheader[11] = (unsigned char)(height>>24);

        f.write(reinterpret_cast<char*>(bmpfileheader), 14);
        f.write(reinterpret_cast<char*>(bmpinfoheader), 40);

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                RGB& c = pixels[height - i - 1][j];
                f.put(c.b); f.put(c.g); f.put(c.r);
            }
            // row padding
            for (size_t p = 0; p < (4 - (width*3)%4)%4; ++p)
                f.put(0);
        }
    }
};

} 

#endif // IMAGE_H
