#pragma once

#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include "Vec3.h"

//一次贝塞尔曲线实现
struct BezierCurve {
	static std::vector<Vec3> createBezierCurve(std::vector<Vec3> point,int totT=100) {//把[0,1]分割成totT，totT越大曲线越精确
		std::vector<Vec3> curve;

		int n = point.size();
		if (n < 3) {
			//不行
		}
		for (int t = 0; t < totT; t++) {
			float nowT = 1.0 / totT*t;
			std::vector<Vec3> nowPoint = point;
			for (int i = 0; i < n - 1; i++) {
				for (int j = 0; j < n - i - 1; j++) {
					nowPoint[j] = bezierCurve(nowPoint[j], nowPoint[j + 1], nowT);
				}
			}
			curve.push_back(nowPoint[0]);
		}
		return curve;
	}
	static Vec3 bezierCurve(Vec3 p0, Vec3 p1, float t) {
		return p0*(1-t) + p1*t;
	}
};
#endif // !BEZIERCURVE_H
