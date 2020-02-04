#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <bits\stdc++.h>
#include "Vec.h"
#include "TXLib.h"

using namespace std;

void init_graphics();
void triangle(Vec3i v0, Vec3i v1, Vec3i v2, COLORREF color, int **zbuffer, const int width, const int height);
void triangle(Vec3i verts[], COLORREF color, int **zbuffer, const int width, const int height);

#endif //__MATRIX_H__