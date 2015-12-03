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
/*
void moveBalls(vector<Ball*> &balls, Octree* octree, float dt) {
for(unsigned int i = 0; i < balls.size(); i++) {
Ball* ball = balls[i];
Vec3f oldPos = ball->pos;
ball->pos += ball->v * dt;
octree->ballMoved(ball, oldPos);
}
}*/

void GLScene::moveAll(vector3 mov, GLOctree* octree)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		vector3 oldPos = meshes[i]->getPositionV();
		vector3 newPos = meshes[i]->getPositionV() - mov + meshes[i]->getVelocity();
		//meshes[i]->setVelocity(mov);
		meshes[i]->setPosition(newPos);

		octree->ballMoved(meshes[i], oldPos);

		/*if(newPos.v[1] > - 10)
		meshes[i]->setPosition(newPos);*/
	}

	handleBallBallCollisions(meshes, octree);
	handleBallWallCollisions(meshes, octree);
}

void GLScene::render()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->render();
	}
}