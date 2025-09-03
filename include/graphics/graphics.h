#pragma once

#include <model/model.h>
#include <graphics/texture.h>
#include <graphics/rasterizer.h>
#include <graphics/canvas.h>
#include <graphics/geometrics.h>
#include <graphics/fragment.h>

class Graphics {
public:
	Graphics(
		Model* _model,
		Texture* _texture,
		Canvas* _canvas,
		Geometrics* _transform
	) : model(_model), transform(_transform), canvas(_canvas), texture(_texture) {}
	~Graphics() {}

	void build(RASTERIZER_MODE type);
	void output(const char* filename);

private:
	Model* model;
	Canvas* canvas;
	Geometrics* transform;
	Texture* texture;
};