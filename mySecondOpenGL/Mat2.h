#pragma once


#ifndef MAT2_H
#define MAT2_H
//���������ʾ����,��������½Ǻ����Ͻ�
#include "Vec3.h"
struct Mat2 {
	Vec3 _min, _max;
	Mat2() {}
	Mat2(Vec3 _min, Vec3 _max) :_min(_min), _max(_max) {}
};
#endif