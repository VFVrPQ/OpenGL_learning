#pragma once

#ifndef AABB_H
#define AABB_H
//AABB碰撞检测，先把物体变成长方体全部加入到objects中，
//再判断点是否在其中一个长方体中，若在的话就表示碰撞
#include <vector>
#include "Mat2.h"
struct AABB {
	std::vector<Mat2> objects;
	AABB() { clear(); }
	void clear() {//清空所有物体
		objects.clear();
	}
	void add(Mat2& obj) {//添加物体，
		objects.push_back(obj);
	}
	void add(Vec3 _min, Vec3 _max) {
		objects.push_back(Mat2(_min, _max));
	}
	bool containPoint(const Vec3& point) const {//判断点是否在物体中
		for (int i = 0; i<int(objects.size()); i++) {
			Vec3 _min = objects[i]._min;
			Vec3 _max = objects[i]._max;
			if (point.x < _min.x) continue;
			if (point.x > _max.x) continue;
			if (point.y < _min.y) continue;
			if (point.y > _max.y) continue;
			if (point.z < _min.z) continue;
			if (point.z > _max.z) continue;
			return true;
		}
		return false;
	}
};
#endif