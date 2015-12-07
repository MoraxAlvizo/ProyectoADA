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

//Moves all of the balls by their velocity times dt
void GLScene::moveAll(float dt, GLOctree* octree, bool isParallel)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		vector3 oldPos = meshes[i]->getPositionV(); 
		vector3 newPos = meshes[i]->getPositionV() + (meshes[i]->getVelocity() * dt);
		meshes[i]->setPosition(newPos);

		if(!isParallel)
			octree->ballMoved(meshes[i], oldPos);

	}
	if(!isParallel)
	{
		handleBallBallCollisions(meshes, octree);
		handleBallWallCollisions(meshes, octree);
	}
	
}

void GLScene::render()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->render();
	}
}