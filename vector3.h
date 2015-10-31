#pragma once

// const used to convert degrees into radians
#define _USE_MATH_DEFINES
#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795

#include <cmath>
#include <stdio.h>
#include "vector4.h"
#include "vector2.h"

class vector2;
class vector3;
class vector4;

/* Operations prototypes*/
float dot(const vector3& a, const vector3& b);
vector3 cross(const vector3& a, const vector3& b);
float get_squared_dist(vector3 from, vector3 to);
vector3 heading_to_direction(float degrees);

class vector3 {


public:
	vector3();
	// create from 3 scalars
	vector3(float x, float y, float z);
	// create from vec2 and a scalar
	vector3(const vector2& vv, float z);
	// create from truncated vec4
	vector3(const vector4& vv);
	// add vector to vector
	vector3 operator+ (const vector3& rhs);
	// add scalar to vector
	vector3 operator+ (float rhs);
	// because user's expect this too
	vector3& operator+= (const vector3& rhs);
	// subtract vector from vector
	vector3 operator- (const vector3& rhs);
	// add vector to vector
	vector3 operator- (float rhs);
	// because users expect this too
	vector3& operator-= (const vector3& rhs);
	// multiply with scalar
	vector3 operator* (float rhs);
	// because users expect this too
	vector3& operator*= (float rhs);
	// divide vector by scalar
	vector3 operator/ (float rhs);
	// because users expect this too
	vector3& operator= (const vector3& rhs);
	// Funtion print
	void print();

	float length();
	float length2();
	vector3 normalise();
	// internal data
	float v[3];
};