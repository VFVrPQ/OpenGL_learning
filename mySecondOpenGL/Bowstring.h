#pragma once


#ifndef BOWSTRING_H
#define BOWSTRING_H

#include <vector>
#include "Vec3.h"
#include "BezierCurve.h"
#include "Trace.h"

//�����ҵ�����
//���������㣨˳���������ң����߸��࣬ȷ��λ�ù�����λ�ã�add�ӵ�
//getCurve�õ��ҵ�����
//draw�����ҵ�����
struct Bowstring {
	std::vector<Vec3> point;
	std::vector<Vec3> bezierCurve;
	void clear() {
		point.clear();
	}
	//�ӵ��ʱ��Ҫ����˳��ӣ����ҵķ���ӣ�����˳ʱ�룬������ʱ�룩
	void add(Vec3&a) {
		point.push_back(a);
	}
	//totT���ƻ����ҵľ��ȣ�Խ��Խ��ȷ
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
