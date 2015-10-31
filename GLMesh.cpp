#include "GLMesh.h"


GLMesh::GLMesh(GLObject* object, vector3 position, GLProgram* program)
{
	matPosition = identity_mat4().translate(position);
	this->object = object;
	this->program = program;
}


GLMesh::~GLMesh(void)
{
}

void GLMesh::render()
{
	glUseProgram (program->getID());
	glUniformMatrix4fv (program->getVariable("model"), 1, GL_FALSE, matPosition.m);
	glDrawArrays (GL_TRIANGLES, 0, object->getPointCount());
	
}

