#pragma once
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

// const used to convert degrees into radians
#define _USE_MATH_DEFINES
#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795

/* stored like this:
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15*/
struct matriz4x4 {
	matriz4x4();
	// note! this is entering components in ROW-major order
	matriz4x4(float a, float b, float c, float d,
		float e, float f, float g, float h,
		float i, float j, float k, float l,
		float mm, float n, float o, float p);
	vector4 operator* (const vector4& rhs);
	matriz4x4 operator* (const matriz4x4& rhs);
	matriz4x4& operator= (const matriz4x4& rhs);

	matriz4x4 inverse();
	matriz4x4 transpose();
	// affine functions
	matriz4x4 translate(const vector3& v);
	matriz4x4 rotate_x_deg(float deg);
	matriz4x4 rotate_y_deg(float deg);
	matriz4x4 rotate_z_deg(float deg);
	matriz4x4 scale(const vector3& v);

	float determinant();
	void print();
	float m[16];
};


matriz4x4 zero_mat4();
matriz4x4 identity_mat4();
