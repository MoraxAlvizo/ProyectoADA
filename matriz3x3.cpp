#include "matriz3x3.h"


matriz3x3::matriz3x3()
{
}

/* note: entered in COLUMNS */
matriz3x3::matriz3x3(
	float a, float b, float c,
	float d, float e, float f,
	float g, float h, float i) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
}


void matriz3x3::print() {
	printf("\n");
	printf("[%.2f][%.2f][%.2f]\n", m[0], m[3], m[6]);
	printf("[%.2f][%.2f][%.2f]\n", m[1], m[4], m[7]);
	printf("[%.2f][%.2f][%.2f]\n", m[2], m[5], m[8]);
}


matriz3x3 zero_mat3() {
	return matriz3x3(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
		);
}

matriz3x3 identity_mat3() {
	return matriz3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
		);
}



