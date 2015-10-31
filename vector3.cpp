#include "vector3.h"




vector3::vector3() {}

vector3::vector3(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vector3::vector3(const vector2& vv, float z) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
}

vector3::vector3(const vector4& vv) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
}

void vector3::print() {
	printf("[%.2f, %.2f, %.2f]\n", v[0], v[1], v[2]);
}

float vector3::length() {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

// squared length
float vector3::length2() {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

// note: proper spelling (hehe)
vector3 vector3::normalise() {
	vector3 vb;
	float l = length();
	if (0.0f == l) {
		return vector3(0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v[0] / l;
	vb.v[1] = v[1] / l;
	vb.v[2] = v[2] / l;
	return vb;
}

vector3 vector3::operator+ (const vector3& rhs) {
	vector3 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}

vector3& vector3::operator+= (const vector3& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}

vector3 vector3::operator- (const vector3& rhs) {
	vector3 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}

vector3& vector3::operator-= (const vector3& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

vector3 vector3::operator+ (float rhs) {
	vector3 vc;
	vc.v[0] = v[0] + rhs;
	vc.v[1] = v[1] + rhs;
	vc.v[2] = v[2] + rhs;
	return vc;
}

vector3 vector3::operator- (float rhs) {
	vector3 vc;
	vc.v[0] = v[0] - rhs;
	vc.v[1] = v[1] - rhs;
	vc.v[2] = v[2] - rhs;
	return vc;
}

vector3 vector3::operator* (float rhs) {
	vector3 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	vc.v[2] = v[2] * rhs;
	return vc;
}

vector3 vector3::operator/ (float rhs) {
	vector3 vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}

vector3& vector3::operator*= (float rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

vector3& vector3::operator= (const vector3& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}


/* Operations for vector 3d*/


float dot(const vector3& a, const vector3& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

vector3 cross(const vector3& a, const vector3& b) {
	float x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	float y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	float z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return vector3(x, y, z);
}

float get_squared_dist(vector3 from, vector3 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	float z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return x + y + z;
}

vector3 heading_to_direction(float degrees) {
	float rad = degrees * ONE_DEG_IN_RAD;
	return vector3(-sinf(rad), 0.0f, -cosf(rad));
}