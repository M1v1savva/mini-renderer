#include <bits\stdc++.h>
#include "Vec.h"

using namespace std;

class Model {
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int id);
	Vec3i face(int id);
private:
	vector<Vec3f> verts;
	vector<Vec3i> faces;
};