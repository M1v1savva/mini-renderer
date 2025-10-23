#include <graphics/canvas.h>

Canvas::Canvas(size_t _width, size_t _height, size_t _depth) 
    : 
    width(_width), height(_height), depth(_depth) {
    if (width <= 0 || height <= 0)
        throw std::runtime_error("Graphics canvas width and height must be positive.");
    map = new RGB*[width];
    for (size_t i = 0; i < width; i++)
        map[i] = new RGB[height];

    zbuffer = new int*[width];
    for (size_t i = 0; i < width; i++) {
        zbuffer[i] = new int[height];
        for (size_t j = 0; j < height; j++)
            zbuffer[i][j] = std::numeric_limits<int>::min();
    }
}

Canvas::~Canvas() {
    for (size_t i = 0; i < width; i++)
        delete[] map[i];
    delete[] map;

    for (size_t i = 0; i < width; i++)
        delete[] zbuffer[i];
    delete[] zbuffer;
}

size_t Canvas::get_width() const { return width; }
size_t Canvas::get_height() const { return height; }
size_t Canvas::get_depth() const { return depth; }
RGB** Canvas::get_pixelmap() const { return map; }

void Canvas::update(int x, int y, int z_bound, const RGB& color) {
    if (in_range(x, y) && zbuffer[x][y] < z_bound) {
        zbuffer[x][y] = z_bound;
        draw_pixel(x, y, color);
    } 
}

bool Canvas::in_range(int x, int y) const {
    return (0 <= x && x < static_cast<int>(width)
            && 0 <= y && y < static_cast<int>(height));
}

void Canvas::draw_pixel(size_t x, size_t y, const RGB& color) {
    map[x][y] = color;
}