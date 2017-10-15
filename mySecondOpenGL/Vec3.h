#pragma once


#ifndef VEC3_H
#define VEC3_H
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
};
#endif