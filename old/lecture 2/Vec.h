#ifndef __VEC_H__
#define __VEC_H__

#include <bits\stdc++.h>

using namespace std;

template<typename T> struct Vec2 {
	union {
		struct {
			T x, y;
		};
		T val[2];
	};

	Vec2() {}
	~Vec2() {}
	template<typename K> Vec2(const Vec2<K> &v);
	Vec2(T _val[]) : x(_val[0]), y(_val[1]) {} 
	Vec2(T _x, T _y) : x(_x), y(_y) {}

	inline Vec2<T> operator + (const Vec2<T> &v) const { return Vec2<T>(x + v.x, y + v.y); }
	inline Vec2<T> operator - (const Vec2<T> &v) const { return Vec2<T>(x - v.x, y - v.y); }
	inline Vec2<T> operator * (const T k)        const { return Vec2<T>(x * k,   y * k); }
	inline T operator * (const Vec2<T> &v) const { return x * v.x + y * v.y; }
	
	T norm() const { return sqrt(x * x + y * y); }
	Vec2<T>& normal() const { return Vec2<T>(-y, x); }
	Vec2<T>& normalize() { (*this) = (*this) * (1. / norm()); return (*this); } 
};

template<typename T> struct Vec3 {
	union {
		struct {
			T x, y, z;
		};
		T val[3];
	};

	Vec3() {}
	~Vec3() {}
	template<typename K> Vec3(const Vec3<K> &v);
	Vec3(T _val[]) : x(_val[0]), y(_val[1]), z(_val[2]) {} 
	Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	inline Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	inline Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<T> operator * (const T k)        const { return Vec3<T>(x * k,   y * k,   z * k); }
	inline T operator * (const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
	inline Vec3<T> operator ^ (const Vec3<T> &v) const { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	T norm() const { return sqrt(x * x + y * y + z * z); }
	Vec3<T>& normalize() { (*this) = (*this) * (1. / norm()); return (*this); } 
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

#endif //__VEC_H__