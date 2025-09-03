#include <graphics/rasterizer.h>
#include <linal/vec.h>

void TexPhRasterizer::rasterize() {
    Vec3i* verts = job-> screen;
    Vec3f* tex = job-> text;
    Vec3f* norms = job-> norm;
    Vec3f light = *(job-> light);
    Texture* texture = job-> texture;
    
    Vec3i v0(verts[0].x, verts[0].y, verts[0].z);
    Vec3i v1(verts[1].x, verts[1].y, verts[1].z);
    Vec3i v2(verts[2].x, verts[2].y, verts[2].z);

    if (v0.y == v1.y && v0.y == v2.y)
        return;
    if (v0.x == v1.x && v0.x == v2.x)
        return;

    if (v0.y > v1.y) {
        std::swap(v0, v1);
        std::swap(tex[0], tex[1]);
        std::swap(norms[0], norms[1]);
    }
    if (v0.y > v2.y) {
        std::swap(v0, v2);
        std::swap(tex[0], tex[2]);
        std::swap(norms[0], norms[2]);
    }
    if (v1.y > v2.y) {
        std::swap(v1, v2);
        std::swap(tex[1], tex[2]);
        std::swap(norms[1], norms[2]);
    }

    float intensity = norms[2] * light;
    if (intensity > 0)
        canvas-> update(v2.x, v2.y, v2.z, texture-> get_color(tex[2]) * intensity);

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

        Vec3f texA = tex[0] + (tex[2] - tex[0]) * alpha;
        Vec3f texB;
        if (!second_half)
            texB = tex[0] + (tex[1] - tex[0]) * beta;
        else
            texB = tex[1] + (tex[2] - tex[1]) * beta;

        Vec3f normA = norms[0] + (norms[2] - norms[0]) * alpha;
        Vec3f normB;
        if (!second_half) 
            normB = norms[0] + (norms[1] - norms[0]) * beta;
        else
            normB = norms[1] + (norms[2] - norms[1]) * beta;

        if (a.x > b.x) {
            std::swap(a, b);
            std::swap(texA, texB);
            std::swap(normA, normB);
        }

        normA.normalize();
        normB.normalize();

        float intensityA = normA * light;
        float intensityB = normB * light;
        if (intensityA > 0) 
            canvas-> update(a.x, a.y, a.z, texture-> get_color(texA) * intensityA);
        if (intensityB > 0)
            canvas-> update(b.x, b.y, b.z, texture-> get_color(texB) * intensityB);
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            Vec3f texC = texA + (texB - texA) * gamma;
            Vec3f normC = normA + (normB - normA) * gamma;

            normC.normalize();
            float intensityC = normC * light;
            if (intensity > 0)
                canvas-> update(x, v0.y + dy, c.z, texture-> get_color(texC) * intensityC);
        }
    }
}