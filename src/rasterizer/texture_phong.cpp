
void Graphics::triangleTexPhong(Vec3i verts[], Vec3f tex[], Vec3f norms[]) {
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
        swap(tex[0], tex[1]);
        swap(norms[0], norms[1]);
    }
    if (v0.y > v2.y) {
        swap(v0, v2);
        swap(tex[0], tex[2]);
        swap(norms[0], norms[2]);
    }
    if (v1.y > v2.y) {
        swap(v1, v2);
        swap(tex[1], tex[2]);
        swap(norms[1], norms[2]);
    }

    //cerr << "kek" << endl;
    float intensity = norms[2] * light;
    if (intensity > 0 && inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
        zbuffer[v2.x][v2.y] = v2.z;
        //cerr << "lol" << endl;
        drawPixel(v2.x, v2.y, getColor(tex[2]) * intensity);
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
            swap(a, b);
            swap(texA, texB);
            swap(normA, normB);
        }

        normA.normalize();
        normB.normalize();

        intensity = normA * light;
        //cerr << "drawing a" << endl;
        if (intensity > 0 && inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, getColor(texA) * intensity);
            //cerr << a.x << ' ' << a.y << endl;
        } 

        intensity = normB * light;
        //cerr << "drawing b" << endl;
        if (intensity > 0 && inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, getColor(texB) * intensity);
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            Vec3f texC = texA + (texB - texA) * gamma;
            Vec3f normC = normA + (normB - normA) * gamma;
            //cerr << "going through the line" << endl;

            normC.normalize();
            intensity = normC * light;
            if (intensity > 0 && inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, getColor(texC) * intensity);
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;   
}