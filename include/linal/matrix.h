#pragma once

#include <vector>

template<typename T, size_t rows, size_t cols> 
struct Matrix {
	static_assert(rows > 0 && cols > 0, "Matrix dimensions must be positive");
	std::vector<std::vector<T> > m;

	Matrix();
	~Matrix() {}
	
	std::vector<T>& operator [] (size_t i) { return m[i]; }
	const std::vector<T>& operator [] (size_t i) const { return m[i]; }

	Matrix<T, rows, cols> operator + (const Matrix<T, rows, cols> &v);
	Matrix<T, rows, cols> operator - (const Matrix<T, rows, cols> &v);
	
	template<size_t cols2> 
	Matrix<T, rows, cols2> operator * (const Matrix<T, cols, cols2> &v) const;
};

#include "matrix.tpp"