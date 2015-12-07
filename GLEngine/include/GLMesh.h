#ifndef GL_MESH_ENGINE
#define GL_MESH_ENGINE

#include "GLObject.h"
#include "GLProgram.h"
#include "MathUtils.h"

// Guarda toda la informacion sobre una pelota en particular. En este caso:
// @object es el apuntador a los buffers de las pelotas.
// @program es el apuntador a los shaders de las pelotas.
// @v es la velocidad que tiene cada pelota en particular.
// @r es el radio de cada pelota.
class GLMesh
{
private:
	GLObject* object;
	GLProgram* program;
	vector3 position;
	matriz4x4 matPosition;
	vector3 v;
	float r;

public:
	GLMesh(GLObject* object, vector3 position, GLProgram* program);
	~GLMesh(void);

	void render();
	vector3 getPositionV(){ return position; }
	matriz4x4 getPosition(){ return matPosition;}
	void setPosition(vector3 position);
	float getRadius(){ return r;}
	vector3 getVelocity(){return v;}
	void setVelocity(vector3 v){this->v = v;}
};

#endif