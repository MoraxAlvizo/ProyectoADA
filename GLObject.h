#ifndef GL_OBJECT_ENGINE
#define GL_OBJECT_ENGINE

#include <gl\glew.h>
#include <string>

using namespace std;

class GLObject
{
private:

	// Atributos
	GLfloat* vp; // array of vertex points
	GLfloat* vn; // array of vertex normals
	GLfloat* vt; // array of texture coordinates
	int		 point_count;
	string   fileName;
	GLuint	 vao;
	GLuint   points_vbo; 
	int		 numbuffer;

	// Metodos
	bool loadObjFile(void);
public:
	GLObject(string ,int );
	~GLObject(void);
	int getPointCount(){ return point_count;}


};

#endif
