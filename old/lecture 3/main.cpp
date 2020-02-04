#include <bits\stdc++.h>
#include "tgaimage.h"
#include "Vec.h"
#include "Model.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const char* filename = "output.tga";

const int width = 800, height = 800, depth = 255;
float zbuffer[width][height];

void triangle(Vec3i v0, Vec3i v1, Vec3i v2, TGAImage &image, TGAColor color) {
	if (v0.y == v1.y && v0.y == v2.y)
		return;
	if (v0.x == v1.x && v0.x == v2.x)
		return;

	if (v0.y > v1.y)
		swap(v0, v1);
	if (v0.y > v2.y)
		swap(v0, v2);
	if (v1.y > v2.y)
		swap(v1, v2);

	if (zbuffer[v2.x][v2.y] < v2.z) {
		zbuffer[v2.x][v2.y] = v2.z;
		image.set(v2.x, v2.y, color);
	}

	for (int dy = 0; dy < v2.y - v0.y; dy++) {
        bool second_half = ((dy > (v1.y - v0.y)) || (v1.y == v0.y));
        
        float alpha = (float)dy / (v2.y - v0.y);
        float beta;
        if (!second_half)
        	beta = (float)dy / (v1.y - v0.y);
        else
        	beta = (float)(dy - (v1.y - v0.y)) / (v2.y - v1.y); 

        Vec3i a = v0 + Vec3f(v2 - v0) * alpha;
        Vec3i b;
        if (!second_half)
        	b = v0 + Vec3f(v1 - v0) * beta;
        else
        	b = v1 + Vec3f(v2 - v1) * beta;

        if (a.x > b.x) 
        	swap(a, b);

        for (int x = a.x; x <= b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;

     		if (zbuffer[x][v0.y + dy] < c.z) {
     			zbuffer[x][v0.y + dy] = c.z;
     			image.set(x, v0.y + dy, color);
        	}
        }
	}
}

void triangle(Vec3i verts[], TGAImage &image, TGAColor color) {
	Vec3i v0(verts[0].x, verts[0].y, verts[0].z);
	Vec3i v1(verts[1].x, verts[1].y, verts[1].z);
	Vec3i v2(verts[2].x, verts[2].y, verts[2].z);
	triangle(v0, v1, v2, image, color);
}

int main() {
	TGAImage image(width, height, TGAImage::RGB);
	
	Model* model = new Model("obj\\african_head.obj");

	for (int i = 0; i < width; i++) 
		for (int j = 0; j < height; j++)
			zbuffer[i][j] = numeric_limits<float>::min();

	const Vec3f light(0, 0, -1);

	for (int i = 0; i < model-> nfaces(); i++) {
		Vec3i face = model-> face(i);
		Vec3i screen[3];
		Vec3f world[3];
		for (int j = 0; j < 3; j++) {
			world[j] = model-> vert(face.val[j]);
			screen[j].x = (world[j].x + 1.) * width / 2;
			screen[j].y = (world[j].y + 1.) * height / 2;
			screen[j].z = (world[j].z + 1.) * depth / 2;
		}

		Vec3f normal = ((world[2] - world[0]) ^ (world[1] - world[0]));
		normal.normalize();
		float intensity = normal * light;
		if (intensity > 0)
			triangle(screen, image, TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255));
			//triangle(screen, image, green);
	}

	image.flip_vertically(); 
	image.write_tga_file(filename);
	return 0;
}