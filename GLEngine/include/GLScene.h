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
	vector<GLMesh*> meshes;
public:
	GLScene(void);
	~GLScene(void);

	void render();
	void addMesh(GLMesh* m);
	void moveAll(vector3 mov, Octree* octree);
	unsigned int getNumMeshes(){ return meshes.size(); }
	void printProperties(){ cerr << "Number of objecs in scene: " << meshes.size(); };
};

#endif
