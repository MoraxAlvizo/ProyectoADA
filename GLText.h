#ifndef GL_TEXT_ENGINE
#define GL_TEXT_ENGINE

#include <ft2build.h>
#include FT_FREETYPE_H
#include <gl\glew.h>
#include "GLProgram.h"
#include <cstring>
#include <MathUtils.h>
#include <map>

using namespace std;

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    vector2		Size;       // Size of glyph
    vector2		Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class GLText
{
private:
	GLuint tex;
	FT_Face face;
	FT_Library ft;
	GLProgram* program;
	GLuint VAO, VBO;
	map<GLchar, Character> Characters;
public:
	GLText(void);
	~GLText(void);

	void render(string text, GLfloat x, GLfloat y, GLfloat scale, vector3 color) ;
};

#endif