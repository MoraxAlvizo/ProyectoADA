#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

#include <cstdio>
vector4::vector4() 
{
}

vector4::vector4(float x, float y, float z, float w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

vector4::vector4(const vector2& vv, float z, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
	v[3] = w;
}

vector4::vector4(const vector3& vv, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
	v[3] = w;
}

void vector4::print() {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v[0], v[1], v[2], v[3]);
}