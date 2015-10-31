#ifndef GL_MESH_ENGINE
#define GL_MESH_ENGINE

#include "GLObject.h"
#include "GLProgram.h"
#include "MathUtils.h"

class GLMesh
{
private:
	GLObject* object;
	GLProgram* program;
	vector3 position;
	matriz4x4 matPosition;
public:
	GLMesh(GLObject* object, vector3 position, GLProgram* program);
	~GLMesh(void);

	void render();
	matriz4x4 getPosition(){ return matPosition;}
};

#endif