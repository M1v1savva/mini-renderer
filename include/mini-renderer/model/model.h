#ifndef __MODEL_H__
#define __MODEL_H__

#include <linal/vec.h>

#include <vector>

namespace mini_renderer {

class Model {
public:
	Model(const char* filename);
	~Model();
	void describe();
	size_t nverts();
	size_t nfaces();
	size_t nvert_norms();
	size_t nface_norms();
	size_t nvert_tex();
	size_t nface_tex();
	Vec3f getVert(size_t id);
	Vec3i getFace(size_t id);
	Vec3f getNorm(size_t id);
	Vec3i getFaceNorm(size_t id);
	Vec3f getTex(size_t id);
	Vec3i getFaceTex(size_t id);
private:
	std::vector<Vec3f> verts;
	std::vector<Vec3i> faces;
	std::vector<Vec3f> vert_norms;
	std::vector<Vec3i> face_norms;
	std::vector<Vec3f> vert_tex;
	std::vector<Vec3i> face_tex;
};

}

#endif //__MODEL_H__