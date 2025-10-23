#include <model/texture.h>

Texture::Texture(const char* filename) { 
    load_tga(filename); 
}

RGB& Texture::get_color(Vec3f uv) {
    float u = std::clamp(uv.x, 0.0f, 0.999999f);
    float v = std::clamp(uv.y, 0.0f, 0.999999f);

    int x = static_cast<int>(u * static_cast<float>(width));
    int y = static_cast<int>(v * static_cast<float>(height));

    x = std::clamp(x, 0, static_cast<int>(width) - 1);
    y = std::clamp(y, 0, static_cast<int>(height) - 1);

    return pixelmap[height - 1 - y][x];
}