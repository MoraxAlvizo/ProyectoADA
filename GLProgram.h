#ifndef GL_PROGRAM_ENGINE
#define GL_PROGRAM_ENGINE

#include <vector>
#include <map>
#include "GLShader.h"
#include "GLUtils.h"

#define NOT_EXIST -1
using namespace std;

class GLProgram
{
public:
	GLProgram(void);
	~GLProgram(void);
	void addShader(GLShader *);
	bool createProgram();
	unsigned int getID(){ return id;}

	void addVariable(string name);
	int getVariable(string name);

private:
	// Atributos
	unsigned int id;
	vector<GLShader*> shaders;
	map<string, int>  variables;

	bool isValid (unsigned int m_programme_idx);
	void printAll (unsigned int m_programme_idx);

};

#endif