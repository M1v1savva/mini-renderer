#include "vec.h"
#include "model.h"
#include "matrix.h"
#include "graphics.h"

#include <iostream>

using namespace std;

int main() {
	char* input_obj = "obj\\african_head.obj";
	Vec3f eye(1, 1, 3);
	Vec3f center(0, 0, 0);
	Vec3f vertical(0, 1, 0);
	Vec3f light(0, 0, -1);

	Graphics g(800, 800, 255);
	g.eye = eye;
	g.center = center;
	g.vertical = vertical;
	g.light = light;
	g.outputFilename = "phong_texture.bmp";
	g.textureFilename = "obj\\african_head_diffuse.bmp";
	g.model = new Model(input_obj);

	g.build(TEXTURE_PHONG);
	return 0;
}