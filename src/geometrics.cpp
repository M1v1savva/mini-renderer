#include <graphics/geometrics.h>
#include <iostream>

void Geometrics::build(size_t width, size_t height, size_t depth) {
    viewport = build_viewport(width, height, depth);
    projection = build_projection();
    view = build_view();
}

Vec3f Geometrics::pass(Vec3f world) {
    return m2v(viewport * projection * view * v2m(world));
}

Matrix<float, 4, 4> Geometrics::build_viewport(size_t width, size_t height, size_t depth) {
    Matrix<float, 4, 4> res;

    res[0][0] = width / 2;
    res[1][1] = -1.f * height / 2;
    res[2][2] = depth / 2;

    res[0][3] = width / 2;
    res[1][3] = height / 2;
    res[2][3] = depth / 2;

    return res;
}

Matrix<float, 4, 4> Geometrics::build_projection() {
    Matrix<float, 4, 4> res;
    res[3][2] = -1. / (eye - center).norm();
    return res;
}

Matrix<float, 4, 4> Geometrics::build_view() {
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
