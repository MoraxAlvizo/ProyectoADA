#include "vector2.h"

vector2::vector2() {}

vector2::vector2(float x, float y) {
	v[0] = x;
	v[1] = y;
}

void vector2::print() {
	printf("[%.2f, %.2f]\n", v[0], v[1]);
}