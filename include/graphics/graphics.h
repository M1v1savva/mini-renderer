#pragma once

#include <model/model.h>
#include <model/image.h>
#include <model/rgb.h>
#include <linal/matrix.h>
#include <linal/vec.h>
#include <graphics/rasterizer.h>

class Graphics {
public:
	Graphics(
		Model* _model,
		Texture* _texture,
		ICanvas* _canvas,
		Geometrics* _transform,
		const char* outputFilename
	) {
		model = _model;
		texture = _texture;
		canvas = _canvas;
		transform = _transform;
		outputFilename = _outputFilename;
	}
	~Graphics();

	void build(RASTERIZER_MODE type);
	void output();
private:
	Model* model;
	ICanvas* canvas;
	Geometrics* geometrics;
	Texture* transform;
	
	char *outputFilename;
};