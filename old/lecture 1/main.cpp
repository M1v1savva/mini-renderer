#include "tgaimage.h"
#include <bits\stdc++.h>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

using namespace std;

void line(int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);

int main() {
	const int width = 800, height = 800;
	TGAImage image(width, height, TGAImage::RGB);
			
	ifstream inp;
	try {
		inp.open("obj\\african_head.obj");
	} catch (ifstream::failure e) {
		cerr << "file is not open" << endl;
	} catch (...) {
		cerr << "some other error" << endl;
	}
	vector<double> xs, ys, zs; 
	int id1, id2, id3, itrash;	
	double x, y, z;
	char trash;
	string type, strash;
	while (inp >> type) {
		if (type == "v") {
			inp >> x >> y >> z;
			x = (x + 1.) * width / 2;
			y = (y + 1.) * height / 2;

			cerr << 'v' << ' ' << x << ' ' << y << endl;

			xs.push_back(x);
			ys.push_back(y);
			zs.push_back(z);
		} else if (type == "f") {
			inp >> id1 >> trash >> itrash >> trash >> itrash;
			inp >> id2 >> trash >> itrash >> trash >> itrash;
			inp >> id3 >> trash >> itrash >> trash >> itrash;
			
			id1--;
			id2--;
			id3--;
			cerr << 'f' << ' ' << id1 << ' ' << id2 << ' ' << id3 << endl;

			line(xs[id1], ys[id1], xs[id2], ys[id2], image, white);
			line(xs[id1], ys[id1], xs[id3], ys[id3], image, white);
			line(xs[id2], ys[id2], xs[id3], ys[id3], image, white);
		} else {
			getline(inp, strash);
		}
	}

	image.flip_vertically(); 
	image.write_tga_file("output.tga");
	return 0;
}

void line(int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color) {
	bool flag = false;
	if (abs(y1 - y2) > abs(x1 - x2)) {
		flag = true;
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}

	int error = 0;
	int derror = abs(y1 - y2) * 2;

	int x = x1, y = y1;
	for (; x <= x2; x++) {
		if (flag)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		error += derror;

		if (error >= x2 - x1) {
			y += ((y1 < y2) ? 1 : -1);
			error -= (x2 - x1) * 2;
		}
	}
}
