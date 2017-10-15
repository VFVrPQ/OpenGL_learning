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
//��ͷ�Ľṹ��,������������
//�����Ѿ����������� �� ��ײ���
struct Arrow {
	float ti = 0.01;//ÿflame��ȥ��ʱ��
	int state;//0:δ������1���Ѵ��������ϣ�2���Ѵ�����ɣ�3����ػ��а�

	Vec3 position, speed, acc;//λ�ã��ٶȣ����ٶ�
	Arrow() {
		position = Vec3(0, 0, 0);
		speed = Vec3(0, 0, 0);
		acc = Vec3(0, -9.8, 0);
		state = 0;
	}
	//����ÿflame��ȥ��ʱ��
	void setTi(float tii) {
		ti = tii;
	}
	//������,pos��ͷ��λ��
	void create(Vec3 pos)
	{
		if (state == 0 || state == 1 || state == 3) {
			position = Vec3(pos.x, pos.y, pos.z);
			speed = Vec3(0, 0, 0);
			acc = Vec3(0, -9.8, 0);
			state = 1;
		}
	}
	//���䣬����spe�ٶȵĴ�С
	void shoot(Vec3 spe) {
		if (state == 1) {
			speed = Vec3(spe.x, spe.y, spe.z);
			state = 2;
		}
	}
	//�߽磬�����߽��ֹͣ
	bool boundary() {
		if (position.y <= 0 && speed.y < 0) return true;//�ڵ������ٶ����� ���������ϰ��
		return false;
	}
	//�ƶ�һ����λʱ��
	//pos�Ǵ�����λ�ã�ֻ��state==1��Ч����aabb����ײ���
	bool move(Vec3 pos,AABB aabb) {
		if (state == 1) {//����������ϣ����ƶ������ҲҪ�ƶ�
			create(pos);
		}
		if (state == 2) {
			if (boundary() || aabb.containPoint(position)) {//�߽�
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
	//��Ϊ�������棬����������Ժ���
	void drawStick(AABB aabb,Vec3 speed) {
		//draw
		if (state != 0) {//�����봴���˲���ʾ
			glColor3f(1.0f, 1.0f, 0.0f);
			glTranslatef(position.x, position.y + 0.05, position.z);
			glutSolidSphere(0.05f, 20, 20);
			glTranslatef(-position.x, -position.y - 0.05, -position.z);

			//��׼��ʱ���������켣
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