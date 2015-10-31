#include "versor.h"


versor::versor()
{
}


versor versor::operator/ (float rhs) {
	versor result;
	result.q[0] = q[0] / rhs;
	result.q[1] = q[1] / rhs;
	result.q[2] = q[2] / rhs;
	result.q[3] = q[3] / rhs;
	return result;
}

versor versor::operator* (float rhs) {
	versor result;
	result.q[0] = q[0] * rhs;
	result.q[1] = q[1] * rhs;
	result.q[2] = q[2] * rhs;
	result.q[3] = q[3] * rhs;
	return result;
}

void versor::print() {
	printf("[%.2f ,%.2f, %.2f, %.2f]\n", q[0], q[1], q[2], q[3]);
}

void versor::operator= (const versor& rhs) {
	
	q[0] = rhs.q[0];
	q[1] = rhs.q[1];
	q[2] = rhs.q[2];
	q[3] = rhs.q[3];

}

versor versor::operator* (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] * q[0] - rhs.q[1] * q[1] -
				  rhs.q[2] * q[2] - rhs.q[3] * q[3];
	result.q[1] = rhs.q[0] * q[1] + rhs.q[1] * q[0] -
			      rhs.q[2] * q[3] + rhs.q[3] * q[2];
	result.q[2] = rhs.q[0] * q[2] + rhs.q[1] * q[3] +
				  rhs.q[2] * q[0] - rhs.q[3] * q[1];
	result.q[3] = rhs.q[0] * q[3] - rhs.q[1] * q[2] +
				  rhs.q[2] * q[1] + rhs.q[3] * q[0];
	// re-normalise in case of mangling
	(*this) = result;
	return normalise();
}

versor versor::operator+ (const versor& rhs) {
	versor result;
	q[0] = rhs.q[0] + q[0];
	q[1] = rhs.q[1] + q[1];
	q[2] = rhs.q[2] + q[2];
	q[3] = rhs.q[3] + q[3];
	// re-normalise in case of mangling
	return normalise();
}

versor quat_from_axis_rad(float radians, float x, float y, float z) {
	versor result;
	result.q[0] = cos(radians / 2.0);
	result.q[1] = sin(radians / 2.0) * x;
	result.q[2] = sin(radians / 2.0) * y;
	result.q[3] = sin(radians / 2.0) * z;
	return result;
}

versor quat_from_axis_deg(float degrees, float x, float y, float z) {
	return quat_from_axis_rad(ONE_DEG_IN_RAD * degrees, x, y, z);
}

matriz4x4 quat_to_mat4(const versor& q) {
	float w = q.q[0];
	float x = q.q[1];
	float y = q.q[2];
	float z = q.q[3];
	return matriz4x4(
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y + 2.0f * w * z,
		2.0f * x * z - 2.0f * w * y,
		0.0f,
		2.0f * x * y - 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z + 2.0f * w * x,
		0.0f,
		2.0f * x * z + 2.0f * w * y,
		2.0f * y * z - 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
		);
}

versor versor::normalise() {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum =
		q[0] * q[0] + q[1] * q[1] +
		q[2] * q[2] + q[3] * q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs(1.0f - sum) < thresh) {
		return (*this);
	}
	float mag = sqrt(sum);
	return (*this) / mag;
}

float versor::dot(const versor& r) {
	return q[0] * r.q[0] + q[1] * r.q[1] + q[2] * r.q[2] + q[3] * r.q[3];
}

versor versor::slerp(versor& r, float t) {
	// angle between q0-q1
	float cos_half_theta = dot(r);
	// as found here http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if (cos_half_theta < 0.0f) {
		for (int i = 0; i < 4; i++) {
			q[i] *= -1.0f;
		}
		cos_half_theta = dot(r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (fabs(cos_half_theta) >= 1.0f) {
		return (*this);
	}
	// Calculate temporary values
	float sin_half_theta = sqrt(1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if (fabs(sin_half_theta) < 0.001f) {
		for (int i = 0; i < 4; i++) {
			result.q[i] = (1.0f - t) * q[i] + t * r.q[i];
		}
		return result;
	}
	float half_theta = acos(cos_half_theta);
	float a = sin((1.0f - t) * half_theta) / sin_half_theta;
	float b = sin(t * half_theta) / sin_half_theta;
	for (int i = 0; i < 4; i++) {
		result.q[i] = q[i] * a + r.q[i] * b;
	}
	return result;
}

void create_versor(float* q, float a, float x, float y, float z) {
	float rad = ONE_DEG_IN_RAD * a;
	q[0] = cosf(rad / 2.0f);
	q[1] = sinf(rad / 2.0f) * x;
	q[2] = sinf(rad / 2.0f) * y;
	q[3] = sinf(rad / 2.0f) * z;
}

/* convert a unit quaternion q to a 4x4 matrix m */
void quat_to_mat4(float* m, float* q) {
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];
	m[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	m[1] = 2.0f * x * y + 2.0f * w * z;
	m[2] = 2.0f * x * z - 2.0f * w * y;
	m[3] = 0.0f;
	m[4] = 2.0f * x * y - 2.0f * w * z;
	m[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	m[6] = 2.0f * y * z + 2.0f * w * x;
	m[7] = 0.0f;
	m[8] = 2.0f * x * z + 2.0f * w * y;
	m[9] = 2.0f * y * z - 2.0f * w * x;
	m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

/* multiply quaternions to get another one. result=R*S */
void mult_quat_quat(float* result, float* r, float* s) {
	result[0] = s[0] * r[0] - s[1] * r[1] -
		s[2] * r[2] - s[3] * r[3];
	result[1] = s[0] * r[1] + s[1] * r[0] -
		s[2] * r[3] + s[3] * r[2];
	result[2] = s[0] * r[2] + s[1] * r[3] +
		s[2] * r[0] - s[3] * r[1];
	result[3] = s[0] * r[3] - s[1] * r[2] +
		s[2] * r[1] + s[3] * r[0];
	// re-normalise in case of mangling
	normalise_quat(result);
}

void normalise_quat(float* q) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs(1.0f - sum) < thresh) {
		return;
	}
	float mag = sqrt(sum);
	for (int i = 0; i < 4; i++) {
		q[i] = q[i] / mag;
	}
}