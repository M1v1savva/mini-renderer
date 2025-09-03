#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>

template<typename T> struct Vec2 {
	union { struct { T x, y; }; T val[2]; };

	Vec2() {}
	~Vec2() {}
	
	template<typename K> 
	Vec2(const Vec2<K> &v);
	
	Vec2(const std::vector<T> &v) : x(v[0]), y(v[1]) {}
	Vec2(T _val[]) : x(_val[0]), y(_val[1]) {} 
	Vec2(T _x, T _y) : x(_x), y(_y) {}

	Vec2<T> operator + (const Vec2<T> &v) const { return Vec2<T>(x + v.x, y + v.y); }
	Vec2<T> operator - (const Vec2<T> &v) const { return Vec2<T>(x - v.x, y - v.y); }
	Vec2<T> operator * (const T k)        const { return Vec2<T>(x * k,   y * k); }
	T       operator * (const Vec2<T> &v) const { return x * v.x + y * v.y; }
	
	T& operator [] (const size_t i) { return val[i]; }
	const T& operator [] (const size_t i) const { return val[i]; }

	T norm() const { return std::sqrt(x * x + y * y); }
	Vec2<T> normal() const { return Vec2<T>(-y, x); }
	Vec2<T>& normalize() {
		T n = norm();
		if (n == 0)
			throw std::runtime_error("Oops, you are trying to normalize a zero-length Vec2<T>.");
		x /= n; y /= n;
		return *this; 
	}
};

template<typename T> struct Vec3 {
	union { struct { T x, y, z; }; T val[3]; };

	Vec3() {}
	~Vec3() {}
	
	template<typename K> 
	Vec3(const Vec3<K> &v);

	Vec3(const std::vector<T> &v) : x(v[0]), y(v[1]), z(v[2]) {}
	Vec3(T _val[]) : x(_val[0]), y(_val[1]), z(_val[2]) {} 
	Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T> operator * (const T k)        const { return Vec3<T>(x * k,   y * k,   z * k); }
	T       operator * (const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
	// Left-hand multiplication
	Vec3<T> operator ^ (const Vec3<T> &v) const { 
		return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); 
	}
	
	T& operator [] (const size_t i) { return val[i]; }
	const T& operator [] (const size_t i) const { return val[i]; }

	T norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<T>& normalize() {
		T n = norm();
		if (n == 0) 
			throw std::runtime_error("Oops, you are trying to normalize a zero-length Vec3<T>.");
		x /= n; y /= n; z /= n; 
		return *this; 
	}
};

template<> template<> 
inline Vec2<int>::Vec2<float>(const Vec2<float> &v) 
    : x(v.x + 0.5), y(v.y + 0.5) {}

template<> template<> 
inline Vec3<int>::Vec3<float>(const Vec3<float> &v) 
    : x(v.x + 0.5), y(v.y + 0.5), z(v.z + 0.5) {}

template<> template<>
inline Vec2<float>::Vec2<int>(const Vec2<int> &v)
    : x(v.x), y(v.y) {}

template<> template<>
inline Vec3<float>::Vec3<int>(const Vec3<int> &v)
    : x(v.x), y(v.y), z(v.z) {}

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;