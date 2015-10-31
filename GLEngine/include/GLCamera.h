#pragma once

#include <iostream>
#include "MathUtils.h"
#include "matriz4x4.h"
#include "vector3.h"
#include "versor.h"

using namespace std;

class GLCamera
{
public:
	GLCamera(float);
	~GLCamera();
	// camera functions
	void		look_at(const vector3& cam_pos, vector3 targ_pos, const vector3& up);
	matriz4x4	perspective(float fovy, float aspect, float near, float far);
	void		moveYawLeft(double);
	void		moveYawRight(double);
	void		movePitchUp(double);
	void		movePitchDown(double);
	void		rollLeft(double);
	void		rollRight(double);
	void		updatePosition(vector3 move);

	// Set funtions
	void setAspect(float aspect){ this->aspect = aspect; }
	void setPosition(vector3 pos);
	
	// Get funtions 
	float getSpeed(){ return cam_speed; }
	float getHeadingSpeed() { return cam_heading_speed; }
	float* getQuaternion() { return quaternion; }
	matriz4x4 getView(){ return view_mat; }
	matriz4x4 getProj(){ return proj_mat; }
	

private:
	float		near;				// clipping plane
	float		far;				// clipping plane
	float		fovy;				// 67 degrees
	float		aspect;				// aspect ratio
	matriz4x4	proj_mat ;			// Proyection matriz
	matriz4x4	view_mat;			// View matrix
	float		cam_speed;			// 1 unit per second
	float		cam_heading_speed;	// 30 degrees per second
	float		cam_heading;		// y-rotation in degrees
	matriz4x4	T;					// Translate matrix
	matriz4x4	R;					// rotation matrix from my maths library. just holds 16 float
	float		quaternion[4];		// make a quaternion representing negated initial camera orientation
	vector3		cam_pos;            // Camera position

	// keep track of some useful vectors that can be used for keyboard movement
	vector4 fwd;
	vector4 rgt;
	vector4 up;

	float cam_yaw; // y-rotation in degrees
	float cam_pitch;
	float cam_roll;

};

