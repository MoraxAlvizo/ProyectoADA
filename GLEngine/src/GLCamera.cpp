#include "GLCamera.h"


GLCamera::GLCamera(float aspect)
{
	near = 0.1f; 
	far = 100.0f; 
	fovy = 67.0f; 
	//aspect = (float)g_gl_width / (float)g_gl_height;
	this->aspect = aspect;
	proj_mat = perspective(fovy, this->aspect, near, far);

	cam_speed = 5.0f; 
	cam_heading_speed = 100.0f;

	cam_heading = 0.0f; 
	cam_pos = vector3(0.0f, 0.0f, 5.0f);
	T = identity_mat4().translate( vector3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]) );

	create_versor(quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
	quat_to_mat4(R.m, quaternion);
	// combine the inverse rotation and transformation to make a view matrix
	view_mat = R * T;

	// keep track of some useful vectors that can be used for keyboard movement
	fwd = vector4(0.0f, 0.0f, -1.0f, 0.0f);
	rgt = vector4(1.0f, 0.0f, 0.0f, 0.0f);
	up = vector4(0.0f, 1.0f, 0.0f, 0.0f);

	cam_yaw = 0.0f; // y-rotation in degrees
	cam_pitch = 0.0f;
	cam_roll = 0.0;
}


GLCamera::~GLCamera()
{
}


/*-----------------------VIRTUAL CAMERA MATRIX FUNCTIONS----------------------*/
// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
void GLCamera::look_at(const vector3& pos, vector3 targ_pos, const vector3& up_v) {

	this->cam_pos = pos; 

	// inverse translation
	matriz4x4 p = identity_mat4();
	p = p.translate(vector3(-pos.v[0], -pos.v[1], -pos.v[2]));
	// distance vector
	vector3 d = targ_pos - pos;
	// forward vector
	vector3 f = d.normalise();
	// right vector
	vector3 r = cross(f, up_v).normalise();
	// real up vector
	vector3 u = cross(r, f).normalise();
	matriz4x4 ori = identity_mat4();
	ori.m[0] = r.v[0];
	ori.m[4] = r.v[1];
	ori.m[8] = r.v[2];
	ori.m[1] = u.v[0];
	ori.m[5] = u.v[1];
	ori.m[9] = u.v[2];
	ori.m[2] = -f.v[0];
	ori.m[6] = -f.v[1];
	ori.m[10] = -f.v[2];
	R = ori;
	T = p;
	view_mat = R * T;

	// recalc axes to suit new orientation
	mat4_to_quat(quaternion, R.m);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up  = R * vector4(0.0, 1.0, 0.0, 0.0);
}

// returns a perspective function mimicking the opengl projection style.
matriz4x4 GLCamera::perspective(float fovy, float aspect, float near, float far) {
	float fov_rad = fovy * ONE_DEG_IN_RAD;
	float range = tan(fov_rad / 2.0f) * near;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
	matriz4x4 m = zero_mat4(); // make sure bottom-right corner is zero
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	m.m[14] = pz;
	m.m[11] = -1.0f;
	return m;
}

void GLCamera::moveYawLeft(double elapsed_seconds)
{
	cam_yaw += getHeadingSpeed() * elapsed_seconds;

	// create a quaternion representing change in heading (the yaw)
	float q_yaw[16];
	create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
	// add yaw rotation to the camera's current orientation
	mult_quat_quat(quaternion, q_yaw, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up  = R * vector4(0.0, 1.0, 0.0, 0.0);

}

void GLCamera::moveYawRight(double elapsed_seconds)
{
	cam_yaw -= cam_heading_speed * elapsed_seconds;

	float q_yaw[16];
	create_versor(q_yaw, cam_yaw, up.v[0], up.v[1], up.v[2]);
	mult_quat_quat(quaternion, q_yaw, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up  = R * vector4(0.0, 1.0, 0.0, 0.0);

}

void GLCamera::movePitchUp(double elapsed_seconds)
{
	cam_pitch += cam_heading_speed * elapsed_seconds;

	float q_pitch[16];
	create_versor(q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
	mult_quat_quat(quaternion, q_pitch, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up = R * vector4(0.0, 1.0, 0.0, 0.0);
}

void GLCamera::movePitchDown(double elapsed_seconds)
{
	cam_pitch -= cam_heading_speed * elapsed_seconds;
	float q_pitch[16];
	create_versor(q_pitch, cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]);
	mult_quat_quat(quaternion, q_pitch, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up = R * vector4(0.0, 1.0, 0.0, 0.0);
}

void GLCamera::rollLeft(double elapsed_seconds)
{
	cam_roll -= cam_heading_speed * elapsed_seconds;

	float q_roll[16];
	create_versor(q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat(quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up = R * vector4(0.0, 1.0, 0.0, 0.0);
}
void GLCamera::rollRight(double elapsed_seconds)
{
	cam_roll += cam_heading_speed * elapsed_seconds;
	float q_roll[16];
	create_versor(q_roll, cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]);
	mult_quat_quat(quaternion, q_roll, quaternion);

	// recalc axes to suit new orientation
	quat_to_mat4(R.m, quaternion);
	fwd = R * vector4(0.0, 0.0, -1.0, 0.0);
	rgt = R * vector4(1.0, 0.0, 0.0, 0.0);
	up = R * vector4(0.0, 1.0, 0.0, 0.0);

}

void GLCamera::updatePosition(vector3 move)
{
	quat_to_mat4(R.m, quaternion);

	// checking for fp errors
	//	printf ("dot fwd . up %f\n", dot (fwd, up));
	//	printf ("dot rgt . up %f\n", dot (rgt, up));
	//	printf ("dot fwd . rgt\n %f", dot (fwd, rgt));

	cam_pos = cam_pos + vector3(fwd) * -move.v[2];
	cam_pos = cam_pos + vector3(up) * move.v[1];
	cam_pos = cam_pos + vector3(rgt) * move.v[0];
	matriz4x4 T = identity_mat4().translate(vector3(cam_pos));

	view_mat = R.inverse() * T.inverse();

	cam_yaw = 0.0f;
	cam_pitch = 0.0f;
	cam_roll = 0.0;

}

void GLCamera::setPosition(vector3 pos)
{ 
	this->cam_pos = pos; 
	T = identity_mat4().translate(vector3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));
	T = identity_mat4().translate( vector3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]) );

	create_versor(quaternion, -cam_heading, 0.0f, 1.0f, 0.0f);
	quat_to_mat4(R.m, quaternion);
	// combine the inverse rotation and transformation to make a view matrix
	view_mat = R * T;

	// keep track of some useful vectors that can be used for keyboard movement
	fwd = vector4(0.0f, 0.0f, -1.0f, 0.0f);
	rgt = vector4(1.0f, 0.0f, 0.0f, 0.0f);
	up = vector4(0.0f, 1.0f, 0.0f, 0.0f);

	cam_yaw = 0.0f; // y-rotation in degrees
	cam_pitch = 0.0f;
	cam_roll = 0.0;

}