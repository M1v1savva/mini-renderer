#pragma once

#include <graphics/texture.h>
#include <linal/vec.h>

struct Fragment {
    Vec3i* screen;
    Vec3f* text;
    float* intensity;
    float* vertIntensity;
    Vec3f* norm;
    Vec3f* light;
    Texture* texture;
};