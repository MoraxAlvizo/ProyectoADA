#include "GLScene.h"


GLScene::GLScene(void)
{
}


GLScene::~GLScene(void)
{
}

void GLScene::addMesh(GLMesh* m)
{
	meshes.push_back(m);
}

void GLScene::render()
{
	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->render();
	}
}