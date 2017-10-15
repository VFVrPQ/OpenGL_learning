#pragma once

#ifndef AABB_H
#define AABB_H
//AABB��ײ��⣬�Ȱ������ɳ�����ȫ�����뵽objects�У�
//���жϵ��Ƿ�������һ���������У����ڵĻ��ͱ�ʾ��ײ
#include <vector>
#include "Mat2.h"
struct AABB {
	std::vector<Mat2> objects;
	AABB() { clear(); }
	void clear() {//�����������
		objects.clear();
	}
	void add(Mat2& obj) {//������壬
		objects.push_back(obj);
	}
	void add(Vec3 _min, Vec3 _max) {
		objects.push_back(Mat2(_min, _max));
	}
	bool containPoint(const Vec3& point) const {//�жϵ��Ƿ���������
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