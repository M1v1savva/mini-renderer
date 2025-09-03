#pragma once

#include <linal/vec.h>

#include <vector>

class Model {
public:
	Model(const char* filename);
	~Model() {}
	void describe();
	inline size_t nverts() { return verts.size(); }
	inline size_t nfaces() { return faces.size(); }
	inline size_t nvert_norms() { return vert_norms.size(); }
	inline size_t nface_norms() { return face_norms.size(); }
	inline size_t nvert_tex() { return vert_tex.size(); }
	inline size_t nface_tex() { return face_tex.size(); }
	inline Vec3f get_vert(size_t id) { return verts[id]; }
	inline Vec3i get_face(size_t id) { return faces[id]; }
	inline Vec3f get_norm(size_t id) { return vert_norms[id]; }
	inline Vec3i get_face_norm(size_t id) { return face_norms[id]; }
	inline Vec3f get_tex(size_t id) { return vert_tex[id]; }
	inline Vec3i get_face_tex(size_t id) { return face_tex[id]; }
private:
	std::vector<Vec3f> verts;
	std::vector<Vec3i> faces;
	std::vector<Vec3f> vert_norms;
	std::vector<Vec3i> face_norms;
	std::vector<Vec3f> vert_tex;
	std::vector<Vec3i> face_tex;
};