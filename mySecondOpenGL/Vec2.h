#pragma once
#ifndef VEC2_H
#define VEC2_H
#include <assert.h>
#include <math.h>
//��ά����
struct Vec2 {
	float x, y;
	Vec2() {}
	Vec2(float x, float y) :x(x), y(y) {}
	void set(float xx, float yy) { x = xx; y = yy; }
	Vec2 operator + (const Vec2& that)const {
		return Vec2(x + that.x, y + that.y);
	}
	Vec2 operator + (const float that[])const {
		assert(sizeof(that) == 8);//��֤that��СΪ2
		return Vec2(x + that[0], y + that[1]);
	}
	float getAngle() {
		return atan2(y, x);
	}
	void rotate(float angle) {
		float cosA = cos(angle);
		float sinA = sin(angle);
		Vec2 tmp = Vec2(x*cosA - y*sinA, x*sinA + y*cosA);
		x = tmp.x;
		y = tmp.y;
	}
};
#endif 