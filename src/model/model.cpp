#include <model/model.h>
#include <model/rgb.h>

#include <iostream>
#include <fstream>
#include <vector>

Model::Model(const char* filename) {
	try {
		std::ifstream inp;
		inp.open(filename);
		std::vector<float> xs, ys, zs; 
		int id[9];	
		float x, y, z;
		char trash;
		std::string type, strash;
		while (inp >> type) {
			if (type == "v") {
				inp >> x >> y >> z;
				verts.push_back(Vec3f(x, y, z));
			} else if (type == "f") {
				inp >> id[0] >> trash >> id[3] >> trash >> id[6];
				inp >> id[1] >> trash >> id[4] >> trash >> id[7];
				inp >> id[2] >> trash >> id[5] >> trash >> id[8];
				for (int i = 0; i < 9; i++) id[i]--;
				faces.push_back(Vec3i(id[0], id[1], id[2]));
				face_norms.push_back(Vec3i(id[6], id[7], id[8]));
				face_tex.push_back(Vec3i(id[3], id[4], id[5]));
			} else if (type == "vn") {
				inp >> x >> y >> z;
				vert_norms.push_back(Vec3f(x, y, z));
			} else if (type == "vt") {
				inp >> x >> y >> z;
				vert_tex.push_back(Vec3f(x, y, z));
			} else {
				std::getline(inp, strash);
			}
		}
	} catch (std::ifstream::failure e) {
		std::cerr << "file is not open" << std::endl;
	} catch (...) {
		std::cerr << "some other error" << std::endl;
	}
}

void Model::describe() {
	using std::cout;
	cout << "Model description:\n";
    cout << "Vertices: " << verts.size() << "\n";
    cout << "Faces: " << faces.size() << "\n";
    cout << "Vertex normals: " << vert_norms.size() << "\n";
    cout << "Face normals: " << face_norms.size() << "\n";
    cout << "Vertex textures: " << vert_tex.size() << "\n";
    cout << "Face textures: " << face_tex.size() << "\n";

    if (!verts.empty()) {
        Vec3f minv = verts[0], maxv = verts[0];
        for (auto& v : verts) {
            for (int i = 0; i < 3; i++) {
                if (v[i] < minv[i]) minv[i] = v[i];
                if (v[i] > maxv[i]) maxv[i] = v[i];
            }
        }
        cout << "Vertex bounds:\n";
        cout << "  Min: (" << minv.x << ", " << minv.y << ", " << minv.z << ")\n";
        cout << "  Max: (" << maxv.x << ", " << maxv.y << ", " << maxv.z << ")\n";
    }
}