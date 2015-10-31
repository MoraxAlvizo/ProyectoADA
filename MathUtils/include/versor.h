#pragma once

#include<cstdio>
#define _USE_MATH_DEFINES
#include<cmath>
#include "matriz4x4.h"


#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795

class versor;
// quaternion functions
versor quat_from_axis_rad(float radians, float x, float y, float z);
versor quat_from_axis_deg(float degrees, float x, float y, float z);
void   create_versor(float* q, float a, float x, float y, float z);
void quat_to_mat4(float* m, float* q);
void mult_quat_quat(float* result, float* r, float* s);
void normalise_quat(float* q);


class versor {

public:
	versor();
	versor operator/ (float rhs);
	versor operator* (float rhs);
	versor operator* (const versor& rhs);
	versor operator+ (const versor& rhs);
	void operator= (const versor& rhs);

	matriz4x4 quat_to_mat4();
	float dot(const versor& r);
	versor slerp(const versor& r);
	versor normalise();
	versor slerp(versor& r, float t);

	void print();
	float q[4];
};

