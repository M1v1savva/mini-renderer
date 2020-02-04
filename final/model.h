#ifndef __MODEL_H__
#define __MODEL_H__

#include "vec.h"
#include <vector>

using namespace std;

class Model {
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	int nvert_norms();
	int nface_norms();
	int nvert_tex();
	int nface_tex();
	Vec3f getVert(int id);
	Vec3i getFace(int id);
	Vec3f getNorm(int id);
	Vec3i getFaceNorm(int id);
	Vec3f getTex(int id);
	Vec3i getFaceTex(int id);
private:
	vector<Vec3f> verts;
	vector<Vec3i> faces;
	vector<Vec3f> vert_norms;
	vector<Vec3i> face_norms;
	vector<Vec3f> vert_tex;
	vector<Vec3i> face_tex;
};

#endif //__MODEL_H__