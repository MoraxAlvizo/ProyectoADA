#ifndef GL_SCENE_ENGINE
#define GL_SCENE_ENGINE

#include <vector>
#include "GLMesh.h"

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
};

#endif
