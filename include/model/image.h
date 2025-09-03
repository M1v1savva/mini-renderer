#pragma once

#include <model/rgb.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <stdexcept>

struct Image {
    RGB** pixelmap;
    size_t width, height;
    
    Image() {}
    ~Image() {
        for (size_t i = 0; i < height; ++i)
            delete[] pixelmap[i];
        delete[] pixelmap;
    }

    void load_bmp(const char* filename) {
        std::ifstream f(filename, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot open file");

        unsigned char header[54];
        f.read(reinterpret_cast<char*>(header), 54);
        width = *reinterpret_cast<int*>(&header[18]);
        height = *reinterpret_cast<int*>(&header[22]);

        int row_padded = (width * 3 + 3) & (~3);
        std::vector<unsigned char> row(row_padded);
        
        pixelmap = new RGB*[height];
        for (size_t i = 0; i < height; ++i)
            pixelmap[i] = new RGB[width];

        for (size_t i = 0; i < height; ++i) {
            f.read(reinterpret_cast<char*>(row.data()), row_padded);
            for (size_t j = 0; j < width; ++j) {
                unsigned char b = row[j * 3 + 0];
                unsigned char g = row[j * 3 + 1];
                unsigned char r = row[j * 3 + 2];
                pixelmap[height - i - 1][j] = RGB(r, g, b);
            }
        }
    }
        
    void save_bmp(const char* filename) {
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
                RGB& c = pixelmap[height - i - 1][j];
                f.put(c.b); 
                f.put(c.g); 
                f.put(c.r);
            }
            // row padding
            for (size_t p = 0; p < (4 - (width * 3) % 4) % 4; ++p)
                f.put(0);
        }
    }
};