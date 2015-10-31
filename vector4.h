#pragma once

class vector3;
class vector2;


class vector4 {

public:
	vector4();
	vector4(float x, float y, float z, float w);
	vector4(const vector2& vv, float z, float w);
	vector4(const vector3& vv, float w);
	void print();
	float v[4];
};

