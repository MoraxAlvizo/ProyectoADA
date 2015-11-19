#include "GLMesh.h"


GLMesh::GLMesh(GLObject* object, vector3 position, GLProgram* program)
{
	matPosition = identity_mat4().translate(position);
	this->position = position;
	this->object = object;
	this->program = program;
	this->r = 0.1;
	this->v = vector3(0, 0.3, 0.1);
}


GLMesh::~GLMesh(void)
{
}

void GLMesh::render()
{
	cout << "position in render() GLMesh.cpp: " << endl;
	cout << position.v[0]<< endl;
	cout << position.v[1] << endl;
	cout << position.v[2] << endl;
	matPosition = identity_mat4().translate(position);
	glUseProgram(program->getID());
	glUniformMatrix4fv(program->getVariable("model"), 1, GL_FALSE, matPosition.m);
	glDrawArrays(GL_TRIANGLES, 0, object->getPointCount());

}

void GLMesh::setPosition(vector3 position)
{
	this->position = position;
}