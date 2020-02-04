#include "Vec.h"
#include "triangle.h"

using namespace std;

void init_graphics() {
    txCreateWindow(800, 800);
}

bool inrange(int x, int y, int width, int height) {
    return (0 <= x && x < width && 0 <= y && y < height);
}

void drawPixel(int x, int y, COLORREF color) {
    txSetPixel(x, y, color);
}

void triangle(Vec3i v0, Vec3i v1, Vec3i v2, COLORREF color, int **zbuffer, const int width, const int height) {
	cerr << v0.x << ' ' << v0.y << ' ' << v0.z << endl;
    cerr << v1.x << ' ' << v1.y << ' ' << v1.z << endl;
    cerr << v2.x << ' ' << v2.y << ' ' << v2.z << endl;    

    if (v0.y == v1.y && v0.y == v2.y)
		return;
	if (v0.x == v1.x && v0.x == v2.x)
		return;

	if (v0.y > v1.y)
		swap(v0, v1);
	if (v0.y > v2.y)
		swap(v0, v2);
	if (v1.y > v2.y)
		swap(v1, v2);

	if (inrange(v2.x, v2.y, width, height) && zbuffer[v2.x][v2.y] < v2.z) {
		zbuffer[v2.x][v2.y] = v2.z;
		drawPixel(v2.x, v2.y, color);
	    cerr << v2.x << ' ' << v2.y << endl;
    } else if (!inrange(v2.x, v2.y, width, height)) {
        cerr << "segment out of boundaries" << endl;
    } else {
        cerr << "behind the stage" << endl;
    }

    cerr << "init main cycle" << endl;
	for (int dy = 0; dy < v2.y - v0.y; dy++) {
        bool second_half = ((dy > (v1.y - v0.y)) || (v1.y == v0.y));
        
        float alpha = (float)dy / (v2.y - v0.y);
        float beta;
        if (!second_half)
        	beta = (float)dy / (v1.y - v0.y);
        else
        	beta = (float)(dy - (v1.y - v0.y)) / (v2.y - v1.y); 

        cerr << "coef init" << endl;

        Vec3i a = v0 + Vec3f(v2 - v0) * alpha;
        Vec3i b;
        if (!second_half)
        	b = v0 + Vec3f(v1 - v0) * beta;
        else
        	b = v1 + Vec3f(v2 - v1) * beta;

        if (a.x > b.x) 
        	swap(a, b);

        cerr << "drawing a" << endl;
        if (inrange(a.x, a.y, width, height) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, color);
            cerr << a.x << ' ' << a.y << endl;
        } else if (!inrange(a.x, a.y, width, height)) {
            cerr << "segment out of boundaries" << endl;
        } else {
            cerr << "behind the stage" << endl;
        }
        cerr << "drawing b" << endl;
        if (inrange(b.x, b.y, width, height) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, color);
            cerr << b.x << ' ' << b.y << endl;
        } else if (!inrange(b.x, b.y, width, height)) {
            cerr << "segment out of boundaries" << endl;
        } else {
            cerr << "behind the stage" << endl;
        }
        cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;

     		if (inrange(x, v0.y + dy, width, height) && zbuffer[x][v0.y + dy] < c.z) {
     		    zbuffer[x][v0.y + dy] = c.z;
     			drawPixel(x, v0.y + dy, color);
                cerr << x << ' ' << (v0.y + dy) << endl;
        	} else if (!inrange(x, v0.y + dy, width, height)) {
                cerr << "segment out of boundaries" << endl;
            } else {
                cerr << "behind the stage" << endl;
            }
        }
	}
    cerr << "finished rast" << endl;
}

void triangle(Vec3i verts[], COLORREF color, int **zbuffer, const int width, const int height) {
	Vec3i v0(verts[0].x, verts[0].y, verts[0].z);
	Vec3i v1(verts[1].x, verts[1].y, verts[1].z);
	Vec3i v2(verts[2].x, verts[2].y, verts[2].z);
	triangle(v0, v1, v2, color, zbuffer, width, height);
}