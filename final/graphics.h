#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "model.h"
#include "matrix.h"
#include "vec.h"
#include "colors.h"
#include "drawer.h"

using namespace std;

enum build_type {
	HOLES,
	WHITE,
	TEXTURE,
	WHITE_GURU,
	WHITE_PHONG,
	TEXTURE_GURU,
	TEXTURE_PHONG
};

class Graphics {
public:
	int width, height, depth;
	Vec3f eye, center, vertical, light;
	Model* model;
	int** zbuffer;
	RGB** map;
	char *outputFilename, *textureFilename;

	Graphics(int _width, int _height, int _depth); 
	~Graphics();

	void build(build_type type);
	void drawMap();
private:
	Matrix<float, 4, 4> viewport, projection, view;
	Vec3f movedLight;
	Drawer textureMap;

	Matrix<float, 4, 4> buildViewport(); 
	Matrix<float, 4, 4> buildProjection();
	Matrix<float, 4, 4> buildView();
	Matrix<float, 4, 1> v2m(Vec3f v);
	Vec3f m2v(Matrix<float, 4, 1> m);

	void initZBuffer();
	void triangleFixedColor(Vec3i verts[], RGB color);
	void triangleWhiteGuru(Vec3i verts[], float intensity[]);
	void triangleWhitePhong(Vec3i verts[], Vec3f norms[]);
	void triangleTex(Vec3i verts[], Vec3f tex[], float intensity);
	void triangleTexGuru(Vec3i verts[], Vec3f tex[], float intensity[]);
	void triangleTexPhong(Vec3i verts[], Vec3f tex[], Vec3f norms[]);
	bool inRange(int x, int y);
	void drawPixel(int x, int y, RGB color);
	RGB getColor(Vec3f v);
};

#endif //__GRAPHICS_H__