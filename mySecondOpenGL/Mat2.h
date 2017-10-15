#pragma once


#ifndef MAT2_H
#define MAT2_H
//用两个点表示矩阵,矩阵的左下角和右上角
#include "Vec3.h"
struct Mat2 {
	Vec3 _min, _max;
	Mat2() {}
	Mat2(Vec3 _min, Vec3 _max) :_min(_min), _max(_max) {}
};
#endif