#include <bits\stdc++.h>
#include "Vec.h"
#include "Model.h"

using namespace std;

Model::Model(const char* filename) {
	try {
		ifstream inp;
		inp.open(filename);
		vector<float> xs, ys, zs; 
		int id1, id2, id3, itrash;	
		float x, y, z;
		char trash;
		string type, strash;
		while (inp >> type) {
			if (type == "v") {
				inp >> x >> y >> z;
				
				cerr << 'v' << ' ' << x << ' ' << y << endl;
				
				verts.push_back(Vec3f(x, y, z));
			} else if (type == "f") {
				inp >> id1 >> trash >> itrash >> trash >> itrash;
				inp >> id2 >> trash >> itrash >> trash >> itrash;
				inp >> id3 >> trash >> itrash >> trash >> itrash;
				
				id1--;
				id2--;
				id3--;
				cerr << 'f' << ' ' << id1 << ' ' << id2 << ' ' << id3 << endl;
			
				faces.push_back(Vec3i(id1, id2, id3));
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

Vec3f Model::vert(int id) {
	return verts[id];
}

Vec3i Model::face(int id) {
	return faces[id];
}