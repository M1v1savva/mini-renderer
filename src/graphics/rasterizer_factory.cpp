#include <graphics/rasterizer.h>
#include <stdexcept>
#include <memory>

std::unique_ptr<IRasterizer> RasterizerFactory::create(RASTERIZER_MODE mode) {
    switch (mode) {
        case FILL:
            return std::make_unique<FillRasterizer>();
        case BINARY:
            return std::make_unique<BinRasterizer>();
        case BINARY_GOURAUD:
            return std::make_unique<BinGuRasterizer>();
        case BINARY_PHONG:
            return std::make_unique<BinPhRasterizer>();
        case TEXTURE:
            return std::make_unique<TexRasterizer>();
        case TEXTURE_GOURAUD:
            return std::make_unique<TexGuRasterizer>();
        case TEXTURE_PHONG:
            return std::make_unique<TexPhRasterizer>();
        default:
            throw std::runtime_error("Unknown rasterizer mode.");
    }
}
