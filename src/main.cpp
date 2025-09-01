#include <linal/vec.h>
#include <models/model.h>
#include <models/image.h>
#include <graphics/graphics.h>

#include <iostream>

int main() {
	const char* INPUT_OBJ = "obj/african_head.obj";
	const char* INPUT_TEX = "obj/african_head_diffuse.bmp";
	const char* OUTPUT_BMP = "phone_texture.bmp";
	const size_t WIDTH = 800, HEIGHT = 800, DEPTH = 255;

	const Vec3f eye(1, 1, 3);
	const Vec3f center(0, 0, 0);
	const Vec3f vertical(0, 1, 0);
	const Vec3f light(0, 0, -1);

	Geometrics* transform = new Geometrics(eye, center, vertical, light);
	ICanvas* canvas = new ICanvas(WIDTH, HEIGH, DEPTH);
	Model* model = new Model(INPUT_OBJ);
	Texture* texture = new Texture(INPUT_TEX);

	Graphics g(
		model,
		texture,
		canvas,
		transform,
		OUTPUT_BMP
	);

	g.build(TEXTURE_PHONG);

	delete transform;
	delete canvas;
	delete model;
	delete texture;
	return 0;
}