#include "util.h"

int g_gl_width = 640;
int g_gl_height = 480;
float screen_x,screen_y;

void glfw_window_size_callback(GLFWwindow* window, int width, int height){

	g_gl_width = width;
	g_gl_height = height;
	/* update any perspective matrices used here */
}

bool restarGlLog(){

	ofstream fe(GL_LOG_FILE);

	if(!fe){
		cerr << "ERROR_ no se puedo abrir " << GL_LOG_FILE << " log file for writing" << endl;
		return false;
	}
	
	time_t now = time(NULL);
	char* date = ctime(&now);

	fe << GL_LOG_FILE " log. local time "  << date;
	fe.close();
	return true;
}

bool glLog( string message, string filename, int line)
{
	ofstream fe(GL_LOG_FILE, ios::app);

	if(!fe){
		cerr << "ERROR_ no se puedo abrir " << GL_LOG_FILE << " log file for writing" << endl;
		return false;
	}
	
	time_t now = time(NULL);
	char* date = ctime(&now);

	fe << filename << ":" << line << " " << message << endl;
	fe.close();
	return true;

}

void glfw_error_callback( int error,const char* descripcion)
{
	cerr << descripcion << endl;
	glLog( descripcion, __FILE__, __LINE__);
}

void _update_fps_counter(GLFWwindow* window)
{
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;

	if( elapsed_seconds > 0.25){
		previous_seconds = current_seconds;
		double fps = (double)frame_count/ elapsed_seconds;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

void _print_shader_info_log(unsigned int shaderIndex){
	int max_lenght = 2048;
	int actual_lenght = 0;

	char log[2048];
	glGetShaderInfoLog( shaderIndex, max_lenght, &actual_lenght, log);
	cout << "shader info log for GL index: " << shaderIndex << ":" << log << endl;

}

void _print_programme_info_log(unsigned int programmeShader){
	int max_lenght = 2048;
	int actual_lenght = 0;

	char log[2048];
	glGetProgramInfoLog( programmeShader, max_lenght, &actual_lenght, log);
	cout << "program info log for GL index: " << programmeShader << ":" << log << endl;
}

void log_gl_params () {
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	glLog ("GL Context Params:", __FILE__, __LINE__);
	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv (params[i], &v);
		sprintf (msg, "%s %i", names[i], v);
		glLog (msg, __FILE__, __LINE__);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv (params[10], v);
	sprintf (msg, "%s %i %i\n", names[10], v[0], v[1]);
	glLog (msg, __FILE__, __LINE__);
	unsigned char s = 0;
	glGetBooleanv (params[11], &s);
	sprintf (msg, "%s %i\n", names[11], (unsigned int)s);
	glLog (msg, __FILE__, __LINE__);
	glLog ("-----------------------------\n", __FILE__, __LINE__);
}
const char* GL_type_to_string (unsigned int type) {
	if (GL_FLOAT == type) {
		return "GL_FLOAT";
	}
	if (GL_FLOAT_VEC2 == type) {
		return "GL_FLOAT_VEC2";
	}
	if (GL_FLOAT_VEC3 == type) {
		return "GL_FLOAT_VEC3";
	}
	if (GL_FLOAT_VEC4 == type) {
		return "GL_FLOAT_VEC4";
	}
	if (GL_FLOAT_MAT2 == type) {
		return "GL_FLOAT_MAT2";
	}
	if (GL_FLOAT_MAT3 == type) {
		return "GL_FLOAT_MAT3";
	}
	if ( GL_FLOAT_MAT4 == type) {
		return "GL_FLOAT_MAT4";
	}
	if (GL_INT == type) {
		return "GL_INT";
	}
	if (GL_BOOL == type) {
		return "GL_BOOL";
	}
	if (GL_SAMPLER_2D == type) {
		return "GL_SAMPLER_2D";
	}
	if (GL_SAMPLER_3D == type) {
		return "GL_SAMPLER_3D";
	}
	if (GL_SAMPLER_CUBE == type) {
		return "GL_SAMPLER_CUBE";
	}
	if (GL_SAMPLER_2D_SHADOW == type) {
		return "GL_SAMPLER_2D_SHADOW";
	}
	return "OTHER";
}

void mouse_click_callback (int b, int s, int mouse_x, int mouse_y){
	/*float x = (2.0f * mouse_x) / (float)g_gl_width -1.0f;
	float y = 1.0f -(2.0f + mouse_y) / g_gl_height;
	float z = 1.0f;

	screen_x = x;
	screen_y = y;

	vec3 ray_nds = vec3(x,y,z);
	vec4 ray_clip = vec4(ray_nds.x,ray_nds.y, -1.0, 1.0); 
	// Matriz de proyeccion
	mat4 proje= perspective(60.0f, (float)g_gl_width / (float)g_gl_width, 0.1f, 100.f); 
	vec4 ray_eye = inverse( proje) * ray_clip;
	ray_eye = vec4 (ray_eye.x,ray_eye.y, -1.0, 0.0);
	vec3 ray_wor = (inverse (view_matrix) * ray_eye).xyz;
	// don't forget to normalise the vector at some point
	ray_wor = normalize (ray_wor);*/
	
}
