#include "graphics.h"
#include "vec.h"
#include "matrix.h"
#include "model.h"

#include <iostream>
#include <limits>

using namespace std;

Graphics::Graphics(int _width, int _height, int _depth) : width(_width), height(_height), depth(_depth) {
    map = new RGB*[width];
    for (int i = 0; i < width; i++) 
        map[i] = new RGB[height];
}

Graphics::~Graphics() {

}

void Graphics::build(build_type type) {
    //cerr << "building started" << endl;
    initZBuffer();
    textureMap.open(textureFilename);

    viewport = buildViewport();
    projection = buildProjection();
    view = buildView();
    movedLight = light;

    for (int i = 0; i < model-> nfaces(); i++) {
        Vec3i face = model-> getFace(i);
        Vec3i screen[3];
        Vec3f world[3];

        Vec3f norm[3];
        Vec3i faceNorm = model-> getFaceNorm(i);
        Vec3f tex[3];
        Vec3i faceTex = model-> getFaceTex(i);
        float vertIntensity[3];

        for (int j = 0; j < 3; j++) {
            Vec3f local = model-> getVert(face.val[j]);
            Vec3f localNorm = model-> getNorm(faceNorm.val[j]);
            Vec3f localTex = model-> getTex(faceTex.val[j]);

            world[j] = local;
            norm[j] = localNorm;
            tex[j] = localTex;

            norm[j].normalize();
            norm[j] = norm[j] * (-1);
            vertIntensity[j] = norm[j] * movedLight;

            //cerr << world[j].x << ' ' << world[j].y << ' ' << world[j].z << endl; 
            
            screen[j] = m2v(viewport * projection * view * v2m(world[j]));
        }

        Vec3f normal = ((world[2] - world[0]) ^ (world[1] - world[0]));
        normal.normalize();
        float intensity = normal * movedLight;
        
        switch (type) {
            case HOLES:                 triangleFixedColor(screen, RGB(0, 255, 0));
                                        break;
            case BINARY:                 if (intensity > 0) triangleFixedColor(screen, RGB(255 * intensity, 255 * intensity, 255 * intensity));
                                        break;
            case BINARY_GOURAUD:            triangleWhiteGuru(screen, vertIntensity);
                                        break;
            case BINARY_PHONG:           triangleWhitePhong(screen, norm);
                                        break;
            case TEXTURE:               if (intensity > 0) triangleTex(screen, tex, intensity);
                                        break;
            case TEXTURE_GOURAUD:          triangleTexGuru(screen, tex, vertIntensity);
                                        break;
            case TEXTURE_PHONG:         triangleTexPhong(screen, tex, norm);
                                        break;
        }
    }

    //cerr << "finished map" << endl;
    drawMap();
}

void Graphics::drawMap() {
    Drawer dr;
    dr.w = width;
    dr.h = height;
    dr.map = map;
    dr.build(outputFilename);
}

Matrix<float, 4, 4> Graphics::buildViewport() {
    Matrix<float, 4, 4> res;
    
    res[0][0] = width / 2;
    res[1][1] = -height / 2;
    res[2][2] = depth / 2;

    res[0][3] = width / 2;
    res[1][3] = height / 2;
    res[2][3] = depth / 2;

    return res;
}

Matrix<float, 4, 4> Graphics::buildProjection() {
    Matrix<float, 4, 4> res;
    res[3][2] = -1. / (eye - center).norm();
    return res;
}

Matrix<float, 4, 4> Graphics::buildView() {
    Matrix<float, 4, 4> rotation;
    Matrix<float, 4, 4> shift;

    Vec3f z = (eye - center).normalize();
    Vec3f x = (vertical ^ z).normalize();
    Vec3f y = (z ^ x).normalize();

    for (int i = 0; i < 3; i++) {
        rotation[0][i] = x[i];
        rotation[1][i] = y[i];
        rotation[2][i] = z[i];
        shift[3][i] = -center[i];
    }

    return rotation * shift;
}

Matrix<float, 4, 1> Graphics::v2m(Vec3f v) {
    Matrix<float, 4, 1> res;
    res[0][0] = v.x;
    res[1][0] = v.y;
    res[2][0] = v.z;
    res[3][0] = 1;
    return res;
} 

Vec3f Graphics::m2v(Matrix<float, 4, 1> m) {
    Vec3f res;
    res.x = m[0][0] / m[3][0];
    res.y = m[1][0] / m[3][0];
    res.z = m[2][0] / m[3][0];
    return res;
}

void Graphics::initZBuffer() {
    zbuffer = new int*[width];
    for (int i = 0; i < width; i++) {
        zbuffer[i] = new int[height];
        for (int j = 0; j < height; j++)
            zbuffer[i][j] = numeric_limits<int>::min();
    }
}

void Graphics::triangleFixedColor(Vec3i verts[], RGB color) {
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

    if (v0.y > v1.y)
        swap(v0, v1);
    if (v0.y > v2.y)
        swap(v0, v2);
    if (v1.y > v2.y)
        swap(v1, v2);

    if (inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
        zbuffer[v2.x][v2.y] = v2.z;
        drawPixel(v2.x, v2.y, color);
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

        if (a.x > b.x) 
            swap(a, b);

        //cerr << "drawing a" << endl;
        if (inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, color);
            //cerr << a.x << ' ' << a.y << endl;
        } 
        //cerr << "drawing b" << endl;
        if (inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, color);
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;// + a;
            //cerr << "going through the line" << endl;

            if (inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, color);
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;
}

void Graphics::triangleWhiteGuru(Vec3i verts[], float intensity[]) {
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

void Graphics::triangleWhitePhong(Vec3i verts[], Vec3f norms[]) {
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
        swap(norms[0], norms[1]);
    }
    if (v0.y > v2.y) {
        swap(v0, v2);
        swap(norms[0], norms[2]);
    }
    if (v1.y > v2.y) {
        swap(v1, v2);
        swap(norms[1], norms[2]);
    }

    //cerr << "kek" << endl;
    float intensity = norms[2] * light;
    if (intensity > 0 && inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
        zbuffer[v2.x][v2.y] = v2.z;
        //cerr << "lol" << endl;
        drawPixel(v2.x, v2.y, RGB(intensity));
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

        Vec3f normA = norms[0] + (norms[2] - norms[0]) * alpha;
        Vec3f normB;
        if (!second_half)
            normB = norms[0] + (norms[1] - norms[0]) * beta;
        else
            normB = norms[1] + (norms[2] - norms[1]) * beta;

        normA.normalize();
        normB.normalize();

        if (a.x > b.x) {
            swap(a, b);
            swap(normA, normB);
        }

        //cerr << "drawing a" << endl;
        intensity = normA * light;
        if (intensity > 0 && inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, RGB(intensity));
            //cerr << a.x << ' ' << a.y << endl;
        } 
        //cerr << "drawing b" << endl;
        intensity = normB * light;
        if (intensity > 0 && inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, RGB(intensity));
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            Vec3f normC = normA + (normB - normA) * gamma;
            normC.normalize();
            intensity = normC * light;
            //cerr << "going through the line" << endl;

            if (intensity > 0 && inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, RGB(intensity));
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;   
}

void Graphics::triangleTex(Vec3i verts[], Vec3f tex[], float intensity) {
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
    }
    if (v0.y > v2.y) {
        swap(v0, v2);
        swap(tex[0], tex[2]);
    }
    if (v1.y > v2.y) {
        swap(v1, v2);
        swap(tex[1], tex[2]);
    }

    //cerr << "kek" << endl;
    if (inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
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

        if (a.x > b.x) {
            swap(a, b);
            swap(texA, texB);
        }

        //cerr << "drawing a" << endl;
        if (inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, getColor(texA) * intensity);
            //cerr << a.x << ' ' << a.y << endl;
        } 
        //cerr << "drawing b" << endl;
        if (inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, getColor(texB) * intensity);
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            Vec3f texC = texA + (texB - texA) * gamma;
            //cerr << "going through the line" << endl;

            if (inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, getColor(texC) * intensity);
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;   
}

void Graphics::triangleTexGuru(Vec3i verts[], Vec3f tex[], float intensity[]) {
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
        swap(intensity[0], intensity[1]);
    }
    if (v0.y > v2.y) {
        swap(v0, v2);
        swap(tex[0], tex[2]);
        swap(intensity[0], intensity[2]);
    }
    if (v1.y > v2.y) {
        swap(v1, v2);
        swap(tex[1], tex[2]);
        swap(intensity[1], intensity[2]);
    }

    //cerr << "kek" << endl;
    if (intensity[2] > 0 && inRange(v2.x, v2.y) && zbuffer[v2.x][v2.y] < v2.z) {
        zbuffer[v2.x][v2.y] = v2.z;
        //cerr << "lol" << endl;
        drawPixel(v2.x, v2.y, getColor(tex[2]) * intensity[2]);
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

        float intensityA = intensity[0] + (intensity[2] - intensity[0]) * alpha;
        float intensityB;
        if (!second_half) 
            intensityB = intensity[0] + (intensity[1] - intensity[0]) * beta;
        else
            intensityB = intensity[1] + (intensity[2] - intensity[1]) * beta;

        if (a.x > b.x) {
            swap(a, b);
            swap(texA, texB);
            swap(intensityA, intensityB);
        }

        //cerr << "drawing a" << endl;
        if (intensityA > 0 && inRange(a.x, a.y) && zbuffer[a.x][a.y] < a.z) {
            zbuffer[a.x][a.y] = a.z;
            drawPixel(a.x, a.y, getColor(texA) * intensityA);
            //cerr << a.x << ' ' << a.y << endl;
        } 
        //cerr << "drawing b" << endl;
        if (intensityB > 0 && inRange(b.x, b.y) && zbuffer[b.x][b.y] < b.z) {
            zbuffer[b.x][b.y] = b.z;
            drawPixel(b.x, b.y, getColor(texB) * intensityB);
            //cerr << b.x << ' ' << b.y << endl;
        } 
        //cerr << "drawing line" << endl;
        for (int x = a.x + 1; x < b.x; x++) {
            float gamma = (float)(x - a.x) / (b.x - a.x);
            Vec3i c = a + Vec3f(b - a) * gamma;
            Vec3f texC = texA + (texB - texA) * gamma;
            float intensityC = intensityA + (intensityB - intensityA) * gamma;
            //cerr << "going through the line" << endl;

            if (intensityC > 0 && inRange(x, v0.y + dy) && zbuffer[x][v0.y + dy] < c.z) {
                zbuffer[x][v0.y + dy] = c.z;
                drawPixel(x, v0.y + dy, getColor(texC) * intensityC);
                //cerr << x << ' ' << (v0.y + dy) << endl;
            } 
        }
    }
    //cerr << "finished rast" << endl;   
}

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

bool Graphics::inRange(int x, int y) {
    return (0 <= x && x < width && 0 <= y && y < height);
}

void Graphics::drawPixel(int x, int y, RGB color) {
    map[x][y] = color;
}

RGB Graphics::getColor(Vec3f v) {
    v.x *= textureMap.w;
    v.y *= textureMap.h;
    Vec3i t = Vec3i(v);
    return textureMap.map[t.x][textureMap.h - 1 - t.y];
}
