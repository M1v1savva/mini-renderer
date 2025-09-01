#pragma once

#include <linal/vec.h>
#include <linal/matrix.h>

class Geometrics {
    Geometrics(Vec3f _eye, Vec3f _center, Vec3f _vertical, Vec3f _light) 
    :
    eye(_eye)
    center(_center)
    vertical(_vertical)
    light(_light)
    {}

    void build(size_t width, size_t height, size_t depth);
    void pass();
    Vec3f get_light() { return light; }
private:
    Vec3f eye, center, vertical, light;
    Vec3f viewport, projection, view;

    Matrix<float, 4, 4> buildViewport(size_t width, size_t height, size_t depth);
    Matrix<float, 4, 4> buildProjection();
    Matrix<float, 4, 4> buildView();

    inline Matrix<float, 4, 1> v2m(Vec3f v) {
        Matrix<float, 4, 1> res;
        res[0][0] = v.x;
        res[1][0] = v.y;
        res[2][0] = v.z;
        res[3][0] = 1;
        return res;
    } 

    inline Vec3f m2v(Matrix<float, 4, 1> m) {
        Vec3f res;
        res.x = m[0][0] / m[3][0];
        res.y = m[1][0] / m[3][0];
        res.z = m[2][0] / m[3][0];
        return res;
    }
}