#pragma once

#include <model/rgb.h>

#include <cstdlib>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>

#pragma pack(push, 1)
struct TGAHeader {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char bitsperpixel;
    char imagedescriptor;
};
#pragma pack(pop)

struct Image {
    RGB** pixelmap;
    size_t width, height;
    
    Image() : pixelmap(nullptr), width(0), height(0) {}
    
    ~Image() {
        if (pixelmap) {
            for (size_t i = 0; i < height; ++i)
                delete[] pixelmap[i];
            delete[] pixelmap;
        }
    }
    
    void load_tga(const char* filename) {
        std::ifstream f(filename, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot open TGA file");
        
        TGAHeader header;
        f.read(reinterpret_cast<char*>(&header), sizeof(header));
        
        if (!f.good()) throw std::runtime_error("Error reading TGA header");
        
        width = header.width;
        height = header.height;
        int bytespp = header.bitsperpixel >> 3;
        
        if (width <= 0 || height <= 0 || (bytespp != 3 && bytespp != 4))
            throw std::runtime_error("Invalid TGA format");
        
        pixelmap = new RGB*[height];
        for (size_t i = 0; i < height; ++i)
            pixelmap[i] = new RGB[width];
        
        if (header.idlength > 0)
            f.seekg(header.idlength, std::ios::cur);
        
        if (header.datatypecode == 2 || header.datatypecode == 3)
            load_tga_uncompressed(f, bytespp);
        else if (header.datatypecode == 10 || header.datatypecode == 11)
            load_tga_rle(f, bytespp);
        else 
            throw std::runtime_error("Unsupported TGA format");
        
            if (!(header.imagedescriptor & 0x20))
            flip_vertically();
    }
    
    void save_tga(const char* filename, bool rle = true) {
        std::ofstream f(filename, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot create TGA file");
        
        TGAHeader header;
        memset(&header, 0, sizeof(header));
        header.bitsperpixel = 24;
        header.width = width;
        header.height = height;
        header.datatypecode = (rle ? 10 : 2);  
        header.imagedescriptor = 0x20; 
        
        f.write(reinterpret_cast<char*>(&header), sizeof(header));
        
        if (!f.good()) throw std::runtime_error("Error writing TGA header");
        
        if (rle) {
            save_tga_rle(f);
        } else {
            save_tga_uncompressed(f);
        }
        const char footer[26] = {0,0,0,0,0,0,0,0,'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.',0};
        f.write(footer, sizeof(footer));
    }
    
    RGB get_pixel(int x, int y) const {
        if (x < 0 || x >= (int)width || y < 0 || y >= (int)height)
            return RGB(0, 0, 0);
        return pixelmap[y][x];
    }
    
private:
    void load_tga_uncompressed(std::ifstream& f, int bytespp) {
        size_t nbytes = width * height * bytespp;
        std::vector<unsigned char> data(nbytes);
        f.read(reinterpret_cast<char*>(data.data()), nbytes);
        
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                size_t idx = (i * width + j) * bytespp;
                unsigned char b = data[idx];
                unsigned char g = (bytespp > 1) ? data[idx + 1] : b;
                unsigned char r = (bytespp > 2) ? data[idx + 2] : b;
                pixelmap[i][j] = RGB(r, g, b);
            }
        }
    }
    
    void load_tga_rle(std::ifstream& f, int bytespp) {
        size_t pixelcount = width * height;
        size_t currentpixel = 0;
        std::vector<unsigned char> colorbuffer(bytespp);
        
        while (currentpixel < pixelcount) {
            unsigned char chunkheader = 0;
            f.read(reinterpret_cast<char*>(&chunkheader), 1);
            
            if (chunkheader < 128) {
                chunkheader++;
                for (int i = 0; i < chunkheader; ++i) {
                    f.read(reinterpret_cast<char*>(colorbuffer.data()), bytespp);
                    size_t y = currentpixel / width;
                    size_t x = currentpixel % width;
                    pixelmap[y][x] = RGB(
                        (bytespp > 2) ? colorbuffer[2] : colorbuffer[0],
                        (bytespp > 1) ? colorbuffer[1] : colorbuffer[0],
                        colorbuffer[0]
                    );
                    currentpixel++;
                }
            } else {
                chunkheader -= 127;
                f.read(reinterpret_cast<char*>(colorbuffer.data()), bytespp);
                RGB color(
                    (bytespp > 2) ? colorbuffer[2] : colorbuffer[0],
                    (bytespp > 1) ? colorbuffer[1] : colorbuffer[0],
                    colorbuffer[0]
                );
                for (int i = 0; i < chunkheader; ++i) {
                    size_t y = currentpixel / width;
                    size_t x = currentpixel % width;
                    pixelmap[y][x] = color;
                    currentpixel++;
                }
            }
        }
    }
    
    void save_tga_uncompressed(std::ofstream& f) {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                RGB& c = pixelmap[i][j];
                f.put(c.b);
                f.put(c.g);
                f.put(c.r);
            }
        }
    }
    
    void save_tga_rle(std::ofstream& f) {
        const int max_chunk_length = 128;
        size_t npixels = width * height;
        size_t curpix = 0;
        
        while (curpix < npixels) {
            size_t chunkstart = curpix;
            size_t y = curpix / width;
            size_t x = curpix % width;
            RGB currentcolor = pixelmap[y][x];
            
            curpix++;
            unsigned char run_length = 1;
            bool raw = true;
            
            while (curpix < npixels && run_length < max_chunk_length) {
                y = curpix / width;
                x = curpix % width;
                bool succ_eq = (pixelmap[y][x].r == currentcolor.r &&
                               pixelmap[y][x].g == currentcolor.g &&
                               pixelmap[y][x].b == currentcolor.b);
                
                curpix++;
                if (run_length == 1) {
                    raw = !succ_eq;
                }
                if (raw && succ_eq) {
                    run_length--;
                    break;
                }
                if (!raw && !succ_eq) {
                    break;
                }
                run_length++;
            }
            
            curpix = chunkstart + run_length;
            
            if (raw) {
                f.put(run_length - 1);
                for (size_t i = 0; i < run_length; ++i) {
                    y = (chunkstart + i) / width;
                    x = (chunkstart + i) % width;
                    RGB& c = pixelmap[y][x];
                    f.put(c.b);
                    f.put(c.g);
                    f.put(c.r);
                }
            } else {
                f.put(run_length + 127);
                f.put(currentcolor.b);
                f.put(currentcolor.g);
                f.put(currentcolor.r);
            }
        }
    }
    
    void flip_vertically() {
        for (size_t i = 0; i < height / 2; ++i) {
            std::swap(pixelmap[i], pixelmap[height - 1 - i]);
        }
    }
};