#ifndef GL_SCENE_ENGINE
#define GL_SCENE_ENGINE

#include <vector>
#include <iostream>
#include "GLMesh.h"
#include "GLOctree.h"

using namespace std;

class GLScene
{
private:
	
public:
	vector<GLMesh*> meshes;

	GLScene(void);
	~GLScene(void);

	void render();
	void addMesh(GLMesh* m);
	void moveAll(float dt, GLOctree* octree, bool isParallel);
	unsigned int getNumMeshes(){ return meshes.size(); }
	void printProperties(){ cerr << "Number of objecs in scene: " << meshes.size(); };
};

#endif
