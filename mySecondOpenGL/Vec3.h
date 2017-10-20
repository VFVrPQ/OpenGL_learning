#pragma once


#ifndef VEC3_H
#define VEC3_H

#include "Vec2.h"
//三维坐标中的点或者向量
struct Vec3 {
	float x, y, z;
	Vec3() {}
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	void set(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}
	Vec3 operator + (const Vec3&a) {
		return Vec3(x + a.x, y + a.y, z + a.z);
	}
	Vec3 operator * (const float&a) {
		return Vec3(x*a, y*a, z*a);
	}
	//a类似相对于原点旋转向量b的方向
	//返回旋转后的结果
	void rotate(const Vec3&b) {
		Vec3 result = Vec3(x,y,z);

		//先在xz平面内旋转（绕y轴旋转）
		Vec2 b_xz(b.x, b.z);
		//得到[0,2pi)的角度
		float angle_xz = b_xz.getAngle();
		//二维旋转
		Vec2 a_xz = Vec2(result.x, result.z);
		a_xz.rotate(angle_xz);
		//结果赋值
		result.x = a_xz.x;
		result.z = a_xz.y;


		//在xy平面内旋转（绕z轴旋转）
		Vec2 b_xy = Vec2(b.x, b.y);

		float angle_xy = b_xy.getAngle();

		Vec2 a_xy = Vec2(result.x, result.y);
		a_xy.rotate(angle_xy);

		result.x = a_xy.x;
		result.y = a_xy.y;

		x = result.x;
		y = result.y;
		z = result.z;
	}
};
#endif