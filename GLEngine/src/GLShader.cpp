#include "GLShader.h"

GLShader::GLShader(string shaderFile, unsigned int typeShader)
{
	this->shaderFile = shaderFile;
	this->typeShader = typeShader;
	codigoGLSL = NULL;
	if( !readShader() )
		cerr << "Error al leer archivo: " << shaderFile << endl;
	compileShader();		
}


GLShader::~GLShader(void)
{
	if(codigoGLSL)
		delete codigoGLSL;
	glDeleteShader(id);
}

bool GLShader::readShader()
{
	ifstream fe(shaderFile);
	string linea;
	string codigo = string();

	if(!fe)
		return false;

	while( getline(fe,linea,'\n') )
		codigo += (linea + "\n");
	codigo += "\0";

	if( codigoGLSL )
		delete codigoGLSL;
	codigoGLSL = new GLchar[codigo.length()];
	for(int index = 0; index < codigo.length(); index++)
		codigoGLSL[index] = codigo.at(index);

	codigoGLSL[codigo.length()] = '\0';

	return true;

}

bool GLShader::compileShader()
{
	const GLchar* codigo = codigoGLSL;
	id = glCreateShader(typeShader);
	glShaderSource(id, 1, &codigo, NULL);
	glCompileShader(id);
	
	int params = -1;
	glGetShaderiv(id, GL_COMPILE_STATUS, &params);
	if(GL_TRUE != params)
	{
		cerr << "Error: GL shader index did not compile: "<< id << endl;
		//_print_shader_info_log(vs);
		return false;
	}
	return true;
}