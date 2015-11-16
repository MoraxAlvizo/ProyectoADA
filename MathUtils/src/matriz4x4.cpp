#include "matriz4x4.h"


matriz4x4::matriz4x4()
{
}

/* note: entered in COLUMNS */
matriz4x4::matriz4x4(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float mm, float n, float o, float p) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
	m[9] = j;
	m[10] = k;
	m[11] = l;
	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}

void matriz4x4::print() {
	printf("\n");
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m[0], m[4], m[8], m[12]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m[1], m[5], m[9], m[13]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m[2], m[6], m[10], m[14]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m[3], m[7], m[11], m[15]);
}

vector4 matriz4x4::operator* (const vector4& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	float y = m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	float z = m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	float w = m[3] * rhs.v[0] +
		m[7] * rhs.v[1] +
		m[11] * rhs.v[2] +
		m[15] * rhs.v[3];
	return vector4(x, y, z, w);
}

matriz4x4 matriz4x4::operator* (const matriz4x4& rhs) {
	matriz4x4 r = zero_mat4();
	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			float sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += rhs.m[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	return r;
}

matriz4x4& matriz4x4::operator= (const matriz4x4& rhs) {
	for (int i = 0; i < 16; i++) {
		m[i] = rhs.m[i];
	}
	return *this;
}


// returns a scalar value with the determinant for a 4x4 matrix
// see http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
float matriz4x4::determinant() {
	return
		m[12] * m[9] * m[6] * m[3] -
		m[8] * m[13] * m[6] * m[3] -
		m[12] * m[5] * m[10] * m[3] +
		m[4] * m[13] * m[10] * m[3] +
		m[8] * m[5] * m[14] * m[3] -
		m[4] * m[9] * m[14] * m[3] -
		m[12] * m[9] * m[2] * m[7] +
		m[8] * m[13] * m[2] * m[7] +
		m[12] * m[1] * m[10] * m[7] -
		m[0] * m[13] * m[10] * m[7] -
		m[8] * m[1] * m[14] * m[7] +
		m[0] * m[9] * m[14] * m[7] +
		m[12] * m[5] * m[2] * m[11] -
		m[4] * m[13] * m[2] * m[11] -
		m[12] * m[1] * m[6] * m[11] +
		m[0] * m[13] * m[6] * m[11] +
		m[4] * m[1] * m[14] * m[11] -
		m[0] * m[5] * m[14] * m[11] -
		m[8] * m[5] * m[2] * m[15] +
		m[4] * m[9] * m[2] * m[15] +
		m[8] * m[1] * m[6] * m[15] -
		m[0] * m[9] * m[6] * m[15] -
		m[4] * m[1] * m[10] * m[15] +
		m[0] * m[5] * m[10] * m[15];
}

/* returns a 16-element array that is the inverse of a 16-element array (4x4
matrix). see http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm */
matriz4x4 matriz4x4::inverse() {
	float det = determinant();
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if (0.0f == det) {
		fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return *this;
	}
	float inv_det = 1.0f / det;

	return matriz4x4(
		inv_det * (
		m[9] * m[14] * m[7] - m[13] * m[10] * m[7] +
		m[13] * m[6] * m[11] - m[5] * m[14] * m[11] -
		m[9] * m[6] * m[15] + m[5] * m[10] * m[15]
		),
		inv_det * (
		m[13] * m[10] * m[3] - m[9] * m[14] * m[3] -
		m[13] * m[2] * m[11] + m[1] * m[14] * m[11] +
		m[9] * m[2] * m[15] - m[1] * m[10] * m[15]
		),
		inv_det * (
		m[5] * m[14] * m[3] - m[13] * m[6] * m[3] +
		m[13] * m[2] * m[7] - m[1] * m[14] * m[7] -
		m[5] * m[2] * m[15] + m[1] * m[6] * m[15]
		),
		inv_det * (
		m[9] * m[6] * m[3] - m[5] * m[10] * m[3] -
		m[9] * m[2] * m[7] + m[1] * m[10] * m[7] +
		m[5] * m[2] * m[11] - m[1] * m[6] * m[11]
		),
		inv_det * (
		m[12] * m[10] * m[7] - m[8] * m[14] * m[7] -
		m[12] * m[6] * m[11] + m[4] * m[14] * m[11] +
		m[8] * m[6] * m[15] - m[4] * m[10] * m[15]
		),
		inv_det * (
		m[8] * m[14] * m[3] - m[12] * m[10] * m[3] +
		m[12] * m[2] * m[11] - m[0] * m[14] * m[11] -
		m[8] * m[2] * m[15] + m[0] * m[10] * m[15]
		),
		inv_det * (
		m[12] * m[6] * m[3] - m[4] * m[14] * m[3] -
		m[12] * m[2] * m[7] + m[0] * m[14] * m[7] +
		m[4] * m[2] * m[15] - m[0] * m[6] * m[15]
		),
		inv_det * (
		m[4] * m[10] * m[3] - m[8] * m[6] * m[3] +
		m[8] * m[2] * m[7] - m[0] * m[10] * m[7] -
		m[4] * m[2] * m[11] + m[0] * m[6] * m[11]
		),
		inv_det * (
		m[8] * m[13] * m[7] - m[12] * m[9] * m[7] +
		m[12] * m[5] * m[11] - m[4] * m[13] * m[11] -
		m[8] * m[5] * m[15] + m[4] * m[9] * m[15]
		),
		inv_det * (
		m[12] * m[9] * m[3] - m[8] * m[13] * m[3] -
		m[12] * m[1] * m[11] + m[0] * m[13] * m[11] +
		m[8] * m[1] * m[15] - m[0] * m[9] * m[15]
		),
		inv_det * (
		m[4] * m[13] * m[3] - m[12] * m[5] * m[3] +
		m[12] * m[1] * m[7] - m[0] * m[13] * m[7] -
		m[4] * m[1] * m[15] + m[0] * m[5] * m[15]
		),
		inv_det * (
		m[8] * m[5] * m[3] - m[4] * m[9] * m[3] -
		m[8] * m[1] * m[7] + m[0] * m[9] * m[7] +
		m[4] * m[1] * m[11] - m[0] * m[5] * m[11]
		),
		inv_det * (
		m[12] * m[9] * m[6] - m[8] * m[13] * m[6] -
		m[12] * m[5] * m[10] + m[4] * m[13] * m[10] +
		m[8] * m[5] * m[14] - m[4] * m[9] * m[14]
		),
		inv_det * (
		m[8] * m[13] * m[2] - m[12] * m[9] * m[2] +
		m[12] * m[1] * m[10] - m[0] * m[13] * m[10] -
		m[8] * m[1] * m[14] + m[0] * m[9] * m[14]
		),
		inv_det * (
		m[12] * m[5] * m[2] - m[4] * m[13] * m[2] -
		m[12] * m[1] * m[6] + m[0] * m[13] * m[6] +
		m[4] * m[1] * m[14] - m[0] * m[5] * m[14]
		),
		inv_det * (
		m[4] * m[9] * m[2] - m[8] * m[5] * m[2] +
		m[8] * m[1] * m[6] - m[0] * m[9] * m[6] -
		m[4] * m[1] * m[10] + m[0] * m[5] * m[10]
		)
		);
}

// returns a 16-element array flipped on the main diagonal
matriz4x4 matriz4x4::transpose() {
	return matriz4x4(
		m[0], m[4], m[8], m[12],
		m[1], m[5], m[9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]
		);
}

/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
// translate a 4d matrix with xyz array
matriz4x4 matriz4x4::translate(const vector3& v) {
	matriz4x4 m_t = identity_mat4();
	m_t.m[12] = v.v[0];
	m_t.m[13] = v.v[1];
	m_t.m[14] = v.v[2];
	return m_t * (*this);
}

// rotate around x axis by an angle in degrees
matriz4x4 matriz4x4::rotate_x_deg(float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	matriz4x4 m_r = identity_mat4();
	m_r.m[5] = cos(rad);
	m_r.m[9] = -sin(rad);
	m_r.m[6] = sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * (*this);
}

// rotate around y axis by an angle in degrees
matriz4x4 matriz4x4::rotate_y_deg(float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	matriz4x4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[8] = sin(rad);
	m_r.m[2] = -sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * (*this);
}

// rotate around z axis by an angle in degrees
matriz4x4 matriz4x4::rotate_z_deg(float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	matriz4x4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[4] = -sin(rad);
	m_r.m[1] = sin(rad);
	m_r.m[5] = cos(rad);
	return m_r * (*this);
}

// scale a matrix by [x, y, z]
matriz4x4 matriz4x4::scale(const vector3& v) {
	matriz4x4 a = identity_mat4();
	a.m[0] = v.v[0];
	a.m[5] = v.v[1];
	a.m[10] = v.v[2];
	return a * (*this);
}

matriz4x4 zero_mat4() {
	return matriz4x4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
		);
}

matriz4x4 identity_mat4() {
	return matriz4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

matriz4x4 ortho(float left,
		float right,
		float  bottom,
		float  top)
{
	float  zNear = -1;
	float  zFar =1;
	return matriz4x4(
		(2) / (right - left), 0.0f, 0.0f, - (right + left) / (right - left),
		0.0f, (2) / (top - bottom), 0.0f, - (top + bottom) / (top - bottom),
		0.0f, 0.0f,- (2) / (zFar - zNear),- (zFar + zNear) / (zFar - zNear),
		0.0f, 0.0f, 0.0f, 1.0f
		);
}
