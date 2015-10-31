#ifndef GL_SHADER_ENGINE
#define GL_SHADER_ENGINE

#include <string>
#include <fstream>
#include <iostream>
#include <gl\glew.h>

using namespace std;

class GLShader
{
public:
	GLShader(string shaderFile, unsigned int typeShader);
	~GLShader(void);
	unsigned int getID(){return id;}

private:
	// Atributos
	string shaderFile;
	GLchar * codigoGLSL;
	unsigned int id;
	unsigned int typeShader;

	// Metodos
	bool readShader();
	bool compileShader();
	

};

#endif
