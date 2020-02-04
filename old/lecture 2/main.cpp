#include <bits\stdc++.h>
#include "tgaimage.h"
#include "Vec.h"
#include "Model.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);

void triangle(Vec2i verts[], TGAImage &image, TGAColor color);

int main() {
	const int width = 800, height = 800;
	TGAImage image(width, height, TGAImage::RGB);
	
	Model* model = new Model("obj\\african_head.obj");

	const Vec3f light(0, 0, -1);

	for (int i = 0; i < model-> nfaces(); i++) {
		Vec3i face = model-> face(i);
		Vec2i screen[3];
		Vec3f world[3];
		for (int j = 0; j < 3; j++) {
			world[j] = model-> vert(face.val[j]);
			screen[j].x = (world[j].x + 1.) * width / 2;
			screen[j].y = (world[j].y + 1.) * height / 2;
		}

		Vec3f normal = ((world[2] - world[0]) ^ (world[1] - world[0]));
		normal.normalize();
		float intensivity = normal * light;
		if (intensivity > 0)
			triangle(screen, image, TGAColor(255 * intensivity, 255 * intensivity, 255 * intensivity, 255));
	}

	image.flip_vertically(); 
	image.write_tga_file("output.tga");
	return 0;
}

void triangle(Vec2i verts[], TGAImage &image, TGAColor color) {
	Vec2i v1 = verts[0], v2 = verts[1], v3 = verts[2];
	
	if (v1.y > v3.y) 
		swap(v1, v3);
	if (v1.y > v2.y) 
		swap(v1, v2);
	if (v2.y > v3.y) 
		swap(v2, v3);

	for (int y = v1.y; y <= v3.y; y++) {
		bool half = (y < v2.y);

		float c1;
		if (half)
			c1 = (float)(y - v1.y + 1) / (v2.y - v1.y);
		else
			c1 = (float)(y - v2.y + 1) / (v3.y - v2.y + 1);
		float c2 = (float)(y - v1.y + 1) / (v3.y - v1.y + 1);

		Vec2f a;
		if (half)
			a = v1 + Vec2f(v2 - v1) * c1;
		else
			a = v2 + Vec2f(v3 - v2) * c1;
		Vec2f b = v1 + Vec2f(v3 - v1) * c2;

		if (a.x > b.x) 
			swap(a, b);

		for (int x = a.x; x <= b.x; x++)
			image.set(x, y, color);
	}
}
