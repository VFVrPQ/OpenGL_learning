#pragma once


#ifndef BOWSTRING_H
#define BOWSTRING_H

#include <vector>
#include "Vec3.h"
#include "BezierCurve.h"
#include "Trace.h"

//弓箭弦的曲线
//可以三个点（顺序是左中右）或者更多，确定位置弓箭的位置，add加点
//getCurve得到弦的曲线
//draw画出弦的曲线
struct Bowstring {
	std::vector<Vec3> point;
	std::vector<Vec3> bezierCurve;
	void clear() {
		point.clear();
	}
	//加点的时候要按照顺序加（沿弦的方向加，可以顺时针，可以逆时针）
	void add(Vec3&a) {
		point.push_back(a);
	}
	//totT控制画出弦的精度，越大越精确
	void getCurve(int totT=100) {
		bezierCurve = BezierCurve::createBezierCurve(point,totT);
	}
	void draw() {
		Trace trace;
		trace.setTrace(bezierCurve);
		trace.draw(1);
	}
};
#endif // !BOWSTRING_H
