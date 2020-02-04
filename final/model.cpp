#include "vec.h"
#include "model.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Model::Model(const char* filename) {
	try {
		ifstream inp;
		inp.open(filename);
		vector<float> xs, ys, zs; 
		int id[9];	
		float x, y, z;
		char trash;
		string type, strash;
		while (inp >> type) {
			if (type == "v") {
				inp >> x >> y >> z;
				
				//cerr << 'v' << ' ' << x << ' ' << y << endl;
				
				verts.push_back(Vec3f(x, y, z));
			} else if (type == "f") {
				inp >> id[0] >> trash >> id[3] >> trash >> id[6];
				inp >> id[1] >> trash >> id[4] >> trash >> id[7];
				inp >> id[2] >> trash >> id[5] >> trash >> id[8];
				
				for (int i = 0; i < 9; i++) id[i]--;

				//cerr << 'f' << ' ' << id1 << ' ' << id2 << ' ' << id3 << endl;
			
				faces.push_back(Vec3i(id[0], id[1], id[2]));
				face_norms.push_back(Vec3i(id[6], id[7], id[8]));
				face_tex.push_back(Vec3i(id[3], id[4], id[5]));
			} else if (type == "vn") {
				inp >> x >> y >> z;

				//cerr << "vn" << ' ' << x << ' ' << y << ' ' << z << endl;

				vert_norms.push_back(Vec3f(x, y, z));
			} else if (type == "vt") {
				inp >> x >> y >> z;

				//cerr << "vt" << ' ' << x << ' ' << y << ' ' << z << endl;

				vert_tex.push_back(Vec3f(x, y, z));
			} else {
				getline(inp, strash);
			}
	}
	} catch (ifstream::failure e) {
		cerr << "file is not open" << endl;
	} catch (...) {
		cerr << "some other error" << endl;
	}
}

Model::~Model() {

}

int Model::nverts() {
	return verts.size();
}

int Model::nfaces() {
	return faces.size();
}

int Model::nvert_norms() {
	return vert_norms.size();
}

int Model::nface_norms() {
	return face_norms.size();
}

int Model::nvert_tex() {
	return vert_tex.size();
}

int Model::nface_tex() {
	return face_tex.size();
}

Vec3f Model::getVert(int id) {
	return verts[id];
}

Vec3i Model::getFace(int id) {
	return faces[id];
}

Vec3f Model::getNorm(int id) {
	return vert_norms[id];
}

Vec3i Model::getFaceNorm(int id) {
	return face_norms[id];
}

Vec3f Model::getTex(int id) {
	return vert_tex[id];
}

Vec3i Model::getFaceTex(int id) {
	return face_tex[id];
}