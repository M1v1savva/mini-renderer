#pragma once

#include <graphics/canvas.h>
#include <graphics/fragment.h>

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

class IRasterizer {
public:
	virtual ~IRasterizer() {}  
	virtual void rasterize() = 0;

	void set_job(Fragment* _job) { job = _job; }
	void set_canvas(Canvas* _canvas) { canvas = _canvas; }	
protected:
	Fragment* job = nullptr;
	Canvas* canvas = nullptr;
};

class RasterizerFactory {
public:
	static std::unique_ptr<IRasterizer> create(RASTERIZER_MODE mode);
};

class FillRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class BinRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class BinGuRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class BinPhRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class TexRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class TexGuRasterizer : public IRasterizer {
public:
	void rasterize() override;
};

class TexPhRasterizer : public IRasterizer {
public:
	void rasterize() override;
};