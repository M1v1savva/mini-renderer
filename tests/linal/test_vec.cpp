#include <linal/vec.h>
#include <randf.h>

#include <gtest/gtest.h>
#include <cmath>

TEST(Vec2Test, BasicOps) {
    float x0 = randf(), y0 = randf(), x1 = randf(), y1 = randf();
    Vec2f a(x0, y0), b(x1, y1);

    EXPECT_FLOAT_EQ((a + b).x, x0 + x1);
    EXPECT_FLOAT_EQ((a + b).y, y0 + y1);

    EXPECT_FLOAT_EQ((a - b).x, x0 - x1);
    EXPECT_FLOAT_EQ((a - b).y, y0 - y1);

    EXPECT_FLOAT_EQ((a * 2.f).x, x0 * 2.f);
    EXPECT_FLOAT_EQ((a * 2.f).y, y0 * 2.f);

    EXPECT_FLOAT_EQ(a * b, x0 * x1 + y0 * y1);

    EXPECT_FLOAT_EQ(a[0], x0);
    EXPECT_FLOAT_EQ(a[1], y0);
    a[0] = 1234567.f;
    EXPECT_FLOAT_EQ(a[0], 1234567.f);

    float n = std::sqrt(x0 * x0 + y0 * y0);
    EXPECT_NEAR(a.norm(), std::sqrt(a[0] * a[0] + y0 * y0), 1e-6);

    const Vec2f perp = a.normal();
    EXPECT_FLOAT_EQ(perp[0], -a.y);
    EXPECT_FLOAT_EQ(perp[1], a.x);

    Vec2f v(3.f, 4.f);
    v.normalize();
    EXPECT_NEAR(v.norm(), 1.f, 1e-6);
}

TEST(Vec3Test, BasicOps) {
    float x0 = randf(), y0 = randf(), z0 = randf();
    float x1 = randf(), y1 = randf(), z1 = randf();
    Vec3f a(x0, y0, z0), b(x1, y1, z1);

    EXPECT_FLOAT_EQ((a + b).x, x0 + x1);
    EXPECT_FLOAT_EQ((a + b).y, y0 + y1);
    EXPECT_FLOAT_EQ((a + b).z, z0 + z1);

    EXPECT_FLOAT_EQ((a - b).x, x0 - x1);
    EXPECT_FLOAT_EQ((a - b).y, y0 - y1);
    EXPECT_FLOAT_EQ((a - b).z, z0 - z1);

    EXPECT_FLOAT_EQ((a * 2.f).x, x0 * 2.f);
    EXPECT_FLOAT_EQ((a * 2.f).y, y0 * 2.f);
    EXPECT_FLOAT_EQ((a * 2.f).z, z0 * 2.f);

    EXPECT_FLOAT_EQ(a * b, x0 * x1 + y0 * y1 + z0 * z1);

    EXPECT_FLOAT_EQ(a[0], x0);
    EXPECT_FLOAT_EQ(a[1], y0);
    EXPECT_FLOAT_EQ(a[2], z0);
    const Vec3i p(1, 2, 3);
    EXPECT_FLOAT_EQ(p[1], 2);

    Vec3f v(1.f, 2.f, 2.f);
    v.normalize();
    EXPECT_NEAR(v.norm(), 1.f, 1e-6);

    Vec3f u(1.f, 0.f, 0.f);
    Vec3f w(0.f, 1.f, 0.f);
    Vec3f c = u ^ w;
    EXPECT_FLOAT_EQ(c.x, 0.f);
    EXPECT_FLOAT_EQ(c.y, 0.f);
    EXPECT_FLOAT_EQ(c.z, 1.f);
}

TEST(VecConversion, Vec2IntFloat) {
    Vec2f f(1.7f, 3.2f);
    Vec2i i(f);
    EXPECT_EQ(i.x, 2); // rounded
    EXPECT_EQ(i.y, 3);
}

TEST(VecConversion, Vec3IntFloat) {
    Vec3f f(1.1f, 2.6f, 3.9f);
    Vec3i i(f);
    EXPECT_EQ(i.x, 1); // rounded
    EXPECT_EQ(i.y, 3);
}