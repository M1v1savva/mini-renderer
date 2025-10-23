#include <graphics/rasterizer.h>

void BinGuRasterizer::rasterize() {
    Vec3i* verts = job-> screen;
    float* intensity = job-> vertIntensity;    

    Vec3i v0(verts[0].x, verts[0].y, verts[0].z);
    Vec3i v1(verts[1].x, verts[1].y, verts[1].z);
    Vec3i v2(verts[2].x, verts[2].y, verts[2].z);
    
    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.x == v1.x && v0.x == v2.x)
        return;

    if (v0.y > v1.y) {
        std::swap(v0, v1);
        std::swap(intensity[0], intensity[1]);
    }
    if (v0.y > v2.y) {
        std::swap(v0, v2);
        std::swap(intensity[0], intensity[2]);
    }
    if (v1.y > v2.y) {
        std::swap(v1, v2);
        std::swap(intensity[1], intensity[2]);
    }

    if (intensity[2] > 0)
        canvas-> update(v2.x, v2.y, v2.z, RGB(intensity[2]));
    
    for (int dy = 0; dy < v2.y - v0.y; dy++) {
        bool second_half = ((dy > (v1.y - v0.y)) || (v1.y == v0.y));
        
        float alpha = (float)dy / (v2.y - v0.y);
        float beta;
        if (!second_half)
            beta = (float)dy / (v1.y - v0.y);
        else
            beta = (float)(dy - (v1.y - v0.y)) / (v2.y - v1.y); 

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
            std::swap(a, b);
            std::swap(intensityA, intensityB);
        }

        if (intensityA > 0)
            canvas-> update(a.x, a.y, a.z, RGB(intensityA));
        if (intensityB > 0)
            canvas-> update(b.x, b.y, b.z, RGB(intensityB));
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            float intensityC = intensityA + (intensityB - intensityA) * gamma;
            if (intensityC > 0)
                canvas-> update(x, a.y, c.z, RGB(intensityC));
        }
    }
}