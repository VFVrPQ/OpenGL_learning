#pragma once


#ifndef VEC3_H
#define VEC3_H

#include "Vec2.h"
//��ά�����еĵ��������
struct Vec3 {
	float x, y, z;
	Vec3() {}
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	void set(float xx, float yy, float zz) {
		x = xx; y = yy; z = zz;
	}
	Vec3 operator + (const Vec3&a) {
		return Vec3(x + a.x, y + a.y, z + a.z);
	}
	Vec3 operator * (const float&a) {
		return Vec3(x*a, y*a, z*a);
	}
	//a���������ԭ����ת����b�ķ���
	//������ת��Ľ��
	void rotate(const Vec3&b) {
		Vec3 result = Vec3(x,y,z);

		//����xzƽ������ת����y����ת��
		Vec2 b_xz(b.x, b.z);
		//�õ�[0,2pi)�ĽǶ�
		float angle_xz = b_xz.getAngle();
		//��ά��ת
		Vec2 a_xz = Vec2(result.x, result.z);
		a_xz.rotate(angle_xz);
		//�����ֵ
		result.x = a_xz.x;
		result.z = a_xz.y;


		//��xyƽ������ת����z����ת��
		Vec2 b_xy = Vec2(b.x, b.y);

		float angle_xy = b_xy.getAngle();

		Vec2 a_xy = Vec2(result.x, result.y);
		a_xy.rotate(angle_xy);

		result.x = a_xy.x;
		result.y = a_xy.y;

		x = result.x;
		y = result.y;
		z = result.z;
	}
};
#endif