#pragma once

#include <model/image.h>
#include <linal/vec.h>

struct Texture : Image {
    Texture(const char* filename) {
        load_bmp(filename);
    }

    RGB& get_color(Vec3f v) {
        v.x *= width;
        v.y *= height;
        Vec3i p(v);
        return pixelmap[height - 1 - p.y][p.x];
    }
};