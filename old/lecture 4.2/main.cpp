#include <bits\stdc++.h>
#include "Vec.h"
#include "Model.h"
#include "Matrix.h"
#include "triangle.h"

using namespace std;

const char* input_obj = "obj\\african_head.obj";

const int width = 800, height = 800, depth = 255;

Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f vertical(0, 1, 0);
Vec3f light(0, 0, -1);

Matrix<float, 4, 4 > projection() {
	Matrix<float, 4, 4> res;
	res[3][2] = -1. / (eye - center).norm();
	return res;
}

Matrix<float, 4, 4> view() {
	Matrix<float, 4, 4> rotation;
	Matrix<float, 4, 4> shift;

	Vec3f z = (eye - center).normalize();
	Vec3f x = (vertical ^ z).normalize();
	Vec3f y = (z ^ x).normalize();

	for (int i = 0; i < 3; i++) {
		rotation[0][i] = x[i];
		rotation[1][i] = y[i];
		rotation[2][i] = z[i];
		shift[3][i] = -center[i];
	}

	return rotation * shift;
}

Matrix<float, 4, 1> v2m(Vec3f v) {
	Matrix<float, 4, 1> res;
	res[0][0] = v.x;
	res[1][0] = v.y;
	res[2][0] = v.z;
	res[3][0] = 1;
	return res;
} 

Vec3f m2v(Matrix<float, 4, 1> m) {
	Vec3f res;
	res.x = m[0][0] / m[3][0];
	res.y = m[1][0] / m[3][0];
	res.z = m[2][0] / m[3][0];
	return res;
}

int main() {
	init_graphics();

	Model *model = new Model(input_obj);
	
	int **zbuffer = new int*[width];
	for (int i = 0; i < width; i++) {
		zbuffer[i] = new int[height];
		for (int j = 0; j < height; j++)
			zbuffer[i][j] = numeric_limits<int>::min();
	}

	Matrix<float, 4, 4> Projection = projection(), View = view();
	Vec3f moved_light = m2v(View * v2m(light));

	for (int i = 0; i < model-> nfaces(); i++) {
		Vec3i face = model-> face(i);
		Vec3i screen[3];
		Vec3f world[3];
		for (int j = 0; j < 3; j++) {
			world[j] = model-> vert(face.val[j]);
			
			world[j] = m2v(Projection * View * v2m(world[j]));
			cerr << world[j].x << ' ' << world[j].y << ' ' << world[j].z << endl; 
			screen[j].x = (world[j].x + 1.) * width / 2;
			screen[j].y = (-world[j].y + 1.) * height / 2;
			screen[j].z = (world[j].z + 1.) * depth / 2;
		}

		Vec3f normal = ((world[2] - world[0]) ^ (world[1] - world[0]));
		normal.normalize();
		float intensity = normal * moved_light;
		if (intensity > 0)
			triangle(screen, RGB(255 * intensity, 255 * intensity, 255 * intensity), zbuffer, width, height);
			//triangle(screen, RGB(rand() % 255,    rand() % 255,    rand() % 255   ), zbuffer, width, height);
			//triangle(screen, TX_GREEN,                                               zbuffer, width, height);
	}

	return 0;
}