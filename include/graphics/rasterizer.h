#pragma once

#include <graphics/icanvas.h>
#include <graphics/iviewport.h>

#include <memory>

enum RASTERIZER_MODE {
    FILL,
	BINARY,
	BINARY_GOURAUD,
	BINARY_PHONG,
	TEXTURE,
	TEXTURE_GOURAUD,
	TEXTURE_PHONG
};

class RasterizerFactory {
public:
	static std::unique_ptr<IRasterizer> create(RASTERIZER_MODE mode);
}

class IRasterizer {
public:
	virtual ~IRasterizer() = default;
	virtual void rasterize() = 0;

	void set_view(IViewport* _view) { view = _view; }
	void set_canvas(ICanvas* _canvas) { canvas = _canvas; }	
protected:
	IViewport* view = nullptr;
	ICanvas* canvas = nullptr;
}

class FillRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class BinRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class BinGuRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class BinPhRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class TexRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class TexGuRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}

class TexPhRasterizer : IRasterizer {
public:
	virtual void rasterize() override;
}
