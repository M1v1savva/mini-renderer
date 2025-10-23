#pragma once

#include <model/image.h>
#include <linal/vec.h>
#include <algorithm>

struct Texture : Image {
    Texture(const char* filename);
    ~Texture() = default;

    RGB& get_color(Vec3f uv);
};