#pragma once

#include<stdio.h>



/* stored like this:
a d g
b e h
c f i */
class matriz3x3 {

public:
	matriz3x3();
	matriz3x3(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i);
	void print();
	float m[9];
};


matriz3x3 zero_mat3();
matriz3x3 identity_mat3();