#include "GLMesh.h"


GLMesh::GLMesh(GLObject* object, vector3 position, GLProgram* program)
{
	matPosition = identity_mat4().translate(position);
	this->position = position;
	this->object = object;
	this->program = program;
	this->r = 1.0;
	this-> v =  vector3(0, 0.25, 0);
}


GLMesh::~GLMesh(void)
{
}

void GLMesh::render()
{
	matPosition = identity_mat4().translate(position);
	glUseProgram (program->getID());
	glUniformMatrix4fv (program->getVariable("model"), 1, GL_FALSE, matPosition.m);
	glDrawArrays (GL_TRIANGLES, 0, object->getPointCount());
	
}

void GLMesh::setPosition(vector3 position)
{
	this->position = position;
}