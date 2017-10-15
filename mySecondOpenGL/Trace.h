#pragma once


#ifndef TRACE_H
#define TRACE_H

#include "Mat2.h"
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//�����ߵĹ켣(��������Ĺ켣��
//���ڽṹ��Arrow��drawStick()�ﱻ����
struct Trace {
	std::vector<Vec3> trace;
	void clear() {
		trace.clear();
	}
	void add(Vec3 &x) {
		trace.push_back(x);
	}
	void setTrace(std::vector<Vec3> &a) {
		trace = a;
	}
	void draw(int step=2) { //step=2�����ߣ�step=1������
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 0; i<int(trace.size()) - 1; i += step) {
			glBegin(GL_LINES);
			glVertex3f(trace[i].x, trace[i].y, trace[i].z);
			glVertex3f(trace[i + 1].x, trace[i + 1].y, trace[i + 1].z);
			glEnd();
		}
		glPopMatrix();
	}
};
#endif