#include <graphics/rasterizer.h>

void BinGuRasterizer::rasterize() {
    Vec3i v0(verts[0].x, verts[0].y, verts[0].z);
    Vec3i v1(verts[1].x, verts[1].y, verts[1].z);
    Vec3i v2(verts[2].x, verts[2].y, verts[2].z);
    
    //cerr << v0.x << ' ' << v0.y << ' ' << v0.z << endl;
    //cerr << v1.x << ' ' << v1.y << ' ' << v1.z << endl;
    //cerr << v2.x << ' ' << v2.y << ' ' << v2.z << endl;    

    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.x == v1.x && v0.x == v2.x)
        return;

    if (v0.y > v1.y) {
        swap(v0, v1);
        swap(intensity[0], intensity[1]);
    }
    if (v0.y > v2.y) {
        swap(v0, v2);
        swap(intensity[0], intensity[2]);
    }
    if (v1.y > v2.y) {
        swap(v1, v2);
        swap(intensity[1], intensity[2]);
    }

    //cerr << "kek" << endl;
    if (intensity[2] > 0 && inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
        zbuffer[v2.x][v2.y] = v2.z;
        //cerr << "lol" << endl;
        drawPixel(v2.x, v2.y, RGB(intensity[2]));
        //cerr << "lol" << endl;
        //cerr << v2.x << ' ' << v2.y << endl;
    } 

    //cerr << "init main cycle" << endl;
    for (int dy = 0; dy < v2.y - v0.y; dy++) {
        bool second_half = ((dy > (v1.y - v0.y)) || (v1.y == v0.y));
        
        float alpha = (float)dy / (v2.y - v0.y);
        float beta;
        if (!second_half)
            beta = (float)dy / (v1.y - v0.y);
        else
            beta = (float)(dy - (v1.y - v0.y)) / (v2.y - v1.y); 

        //cerr << "coef init" << endl;

        Vec3i a = v0 + Vec3f(v2 - v0) * alpha;
        Vec3i b;

        if (!second_half)
            b = v0 + Vec3f(v1 - v0) * beta;
        else
            b = v1 + Vec3f(v2 - v1) * beta;

        float intensityA = intensity[0] + (intensity[2] - intensity[0]) * alpha;
        float intensityB;
        if (!second_half)
            intensityB = intensity[0] + (intensity[1] - intensity[0]) * beta;
        else
            intensityB = intensity[1] + (intensity[2] - intensity[1]) * beta;

        if (a.x > b.x) {
            swap(a, b);
            swap(intensityA, intensityB);
        }

        //cerr << "drawing a" << endl;
        if (intensityA > 0 && inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, RGB(intensityA));
            //cerr << a.x << ' ' << a.y << endl;
        } 
        //cerr << "drawing b" << endl;
        if (intensityB > 0 && inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, RGB(intensityB));
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            float intensityC = intensityA + (intensityB - intensityA) * gamma;
            //cerr << "going through the line" << endl;

            if (intensityC > 0 && inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, RGB(intensityC));
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;
}