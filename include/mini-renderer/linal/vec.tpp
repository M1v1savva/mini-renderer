#include <stdexcept>

namespace mini_renderer {

template<typename T>
Vec2<T>& Vec2<T>::normalize() {
    T n = norm();
    if (n == 0)
        throw std::runtime_error("Oops, you are trying to normalize a zero-length Vec2<T>.");
    x /= n;
    y /= n;
    return *this; 
}

template<typename T>
inline Vec3<T> Vec3<T>::operator ^ (const Vec3<T> &v) const { 
	return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); 
}

template<typename T>
Vec3<T>& Vec3<T>::normalize() {
    T n = norm();
    if (n == 0) 
        throw std::runtime_error("Oops, you are trying to normalize a zero-length Vec3<T>.");
    x /= n;
    y /= n;
    z /= n; 
    return *this; 
}

template<> template<> 
Vec2<int>::Vec2<float>(const Vec2<float> &v) 
    : 
    x(v.x + 0.5), 
    y(v.y + 0.5) 
    {}

template<> template<> 
Vec3<int>::Vec3<float>(const Vec3<float> &v) 
    : 
    x(v.x + 0.5), 
    y(v.y + 0.5), 
    z(v.z + 0.5) 
    {}

}