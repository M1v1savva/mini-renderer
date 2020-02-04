#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include "vec.h"

using namespace std;

template<typename T, int rows, int cols> struct Matrix {
	vector<vector<T> > m;

	Matrix() {
		m = vector<vector<T> >(rows, vector<T>(cols, 0));
		for (int i = 0; i < min(rows, cols); i++)
			m[i][i] = 1;
	}

	~Matrix() {}
	
	vector<T>& operator [] (const int &i) {
		return m[i];
	}

	Matrix<T, rows, cols> operator + (const Matrix<T, rows, cols> &v) {
		Matrix<T, rows, cols> result;
		for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result.m[i][j] = m[i][j] + v.m[i][j];
		return result;
	}

	Matrix<T, rows, cols> operator - (const Matrix<T, rows, cols> &v) {
		Matrix<T, rows, cols> result;
		for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result.m[i][j] = m[i][j] - v.m[i][j];	
		return result;
	}

	template<int cols2> Matrix<T, rows, cols2> operator * (const Matrix<T, cols, cols2> &v) {
		Matrix<T, rows, cols2> result;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols2; j++) {
				result.m[i][j] = 0;
				for (int k = 0; k < cols; k++)
					result.m[i][j] += m[i][k] * v.m[k][j];
			}
		}
		return result;
	}
};

#endif //__MATRIX_H__