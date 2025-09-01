#include <model/image.h>
#include <linal/vec.h>

struct Texture : Image {
    Image texture;

    Texture(const char* filename) {
        texture = Image();
        texture.loadBMP(filename);
    }

    RGB& Graphics::getColor(Vec3f v) {
        v.x *= width;
        v.y *= height;
        return pixels[v.x][v.y]; // textureMap.h - 1 - t.y
    }

}