#pragma once

#ifndef ARROW_H
#define ARROW_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "AABB.h"
#include "Trace.h"
//箭头的结构体,用于物理引擎
//现在已经做好了重力 和 碰撞检测
struct Arrow {
	float ti = 0.01;//每flame过去的时间
	int state;//0:未创建；1：已创建在手上；2：已创建起飞；3：落地或中靶

	Vec3 position, speed, acc;//位置，速度，加速度
	Arrow() {
		position = Vec3(0, 0, 0);
		speed = Vec3(0, 0, 0);
		acc = Vec3(0, -9.8, 0);
		state = 0;
	}
	//设置每flame过去的时间
	void setTi(float tii) {
		ti = tii;
	}
	//创建箭,pos箭头的位置
	void create(Vec3 pos)
	{
		if (state == 0 || state == 1 || state == 3) {
			position = Vec3(pos.x, pos.y, pos.z);
			speed = Vec3(0, 0, 0);
			acc = Vec3(0, -9.8, 0);
			state = 1;
		}
	}
	//发射，传入spe速度的大小
	void shoot(Vec3 spe) {
		if (state == 1) {
			speed = Vec3(spe.x, spe.y, spe.z);
			state = 2;
		}
	}
	//边界，碰到边界就停止
	bool boundary() {
		if (position.y <= 0 && speed.y < 0) return true;//在地面且速度向下 或者碰到障碍物。
		return false;
	}
	//移动一个单位时间
	//pos是创建的位置（只在state==1有效），aabb是碰撞检测
	bool move(Vec3 pos,AABB aabb) {
		if (state == 1) {//如果拿在手上，手移动，则箭也要移动
			create(pos);
		}
		if (state == 2) {
			if (boundary() || aabb.containPoint(position)) {//边界
															//position.y = 1;
				state = 3;
			}
			else {
				changePosition();
				changeSpeed();
				return true;
			}
		}
		return false;
	}
	//作为物理引擎，这个函数可以忽略
	void drawStick(AABB aabb,Vec3 speed) {
		//draw
		if (state != 0) {//箭必须创建了才显示
			glColor3f(1.0f, 1.0f, 0.0f);
			glTranslatef(position.x, position.y + 0.05, position.z);
			glutSolidSphere(0.05f, 20, 20);
			glTranslatef(-position.x, -position.y - 0.05, -position.z);

			//瞄准的时候辅助画出轨迹
			if (state == 1 || state == 2) {//state == 1
				Trace mTrace;
				Arrow tmpArrow = *this;
				tmpArrow.state = 2;
				tmpArrow.speed = speed;
				do {
					mTrace.add(tmpArrow.position);
				} while (tmpArrow.move(Vec3(0, 0, 0), aabb));
				mTrace.draw();
			}
		}
	}
	void changePosition() {
		position.x += speed.x *ti;
		position.y += speed.y *ti;
		position.z += speed.z *ti;
	}
	void changeSpeed() {
		speed.x += acc.x *ti;
		speed.y += acc.y *ti;
		speed.z += acc.z *ti;
	}
};
#endif // ! ARROW_H