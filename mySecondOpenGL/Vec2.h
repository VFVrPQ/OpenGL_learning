#pragma once
#ifndef VEC2_H
#define VEC2_H
#include <assert.h>
//二维坐标
struct Vec2 {
	float x, y;
	Vec2() {}
	Vec2(float x, float y) :x(x), y(y) {}
	void set(float xx, float yy) { x = xx; y = yy; }
	Vec2 operator + (const Vec2& that)const {
		return Vec2(x + that.x, y + that.y);
	}
	Vec2 operator + (const float that[])const {
		assert(sizeof(that) == 8);//保证that大小为2
		return Vec2(x + that[0], y + that[1]);
	}
};
#endif 