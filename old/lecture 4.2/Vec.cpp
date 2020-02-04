#include "Vec.h"

using namespace std;

template<> template<> Vec2f::Vec2<>(const Vec2f &v) : x(v.x), y(v.y) {}
template<> template<> Vec2i::Vec2<>(const Vec2i &v) : x(v.x), y(v.y) {}
template<> template<> Vec3f::Vec3<>(const Vec3f &v) : x(v.x), y(v.y), z(v.z) {}
template<> template<> Vec3i::Vec3<>(const Vec3i &v) : x(v.x), y(v.y), z(v.z) {}

template<> template<> Vec2f::Vec2<>(const Vec2i &v) : x(v.x), y(v.y) {}
template<> template<> Vec2i::Vec2<>(const Vec2f &v) : x(v.x + 0.5), y(v.y + 0.5) {}
template<> template<> Vec3f::Vec3<>(const Vec3i &v) : x(v.x), y(v.y), z(v.z) {}
template<> template<> Vec3i::Vec3<>(const Vec3f &v) : x(v.x + 0.5), y(v.y + 0.5), z(v.z + 0.5) {}