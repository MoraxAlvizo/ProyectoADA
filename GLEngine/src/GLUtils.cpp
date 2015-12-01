#include "GLUtils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#define GL_LOG_FILE "gl.log"
#define MAX_SHADER_LENGTH 262144

int g_gl_width = 600;
int g_gl_height = 600;
GLFWwindow* g_window;
/*--------------------------------LOG FUNCTIONS-------------------------------*/
bool restart_gl_log () {
	FILE* file = fopen (GL_LOG_FILE, "w");
	if (!file) {
		fprintf (
			stderr,
			"ERROR: could not open GL_LOG_FILE log file %s for writing\n",
			GL_LOG_FILE
		);
		return false;
	}
	time_t now = time (NULL);
	char* date = ctime (&now);
	fprintf (file, "GL_LOG_FILE log. local time %s\n", date);
	fclose (file);
	return true;
}

bool gl_log (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf (
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

/* same as gl_log except also prints to stderr */
bool gl_log_err (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf (
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	va_start (argptr, message);
	vfprintf (stderr, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

bool start_gl () {
	gl_log ("starting GLFW %s", glfwGetVersionString ());
	
	glfwSetErrorCallback (glfw_error_callback);
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	// uncomment these lines if on Apple OS X
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	/* Full screen we need to configure that later*/
	/*GLFWmonitor* mon = glfwGetPrimaryMonitor ();
	const GLFWvidmode* vmode = glfwGetVideoMode (mon);
	g_window = glfwCreateWindow (
		vmode->width, vmode->height, "Extended GL Init", mon, NULL
	);
	g_gl_width = vmode->width;
	g_gl_height = vmode->height;*/
	
	g_window = glfwCreateWindow (
		g_gl_width, g_gl_height, "Extended Init.", NULL, NULL
	);
	if (!g_window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}
	glfwSetWindowSizeCallback (g_window, glfw_window_size_callback);
	glfwMakeContextCurrent (g_window);
	
	glfwWindowHint (GLFW_SAMPLES, 4);
	
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);
	gl_log ("renderer: %s\nversion: %s\n", renderer, version);
	
	return true;
}

void glfw_error_callback (int error, const char* description) {
	fputs (description, stderr);
	gl_log_err ("%s\n", description);
}
// a call-back function
void glfw_window_size_callback (GLFWwindow* window, int width, int height) {
	g_gl_width = width;
	g_gl_height = height;
	printf ("width %i height %i\n", width, height);
	/* update any perspective matrices used here */
}

void _update_fps_counter (GLFWwindow* window) {
	static double previous_seconds = glfwGetTime ();
	static int frame_count;
	double current_seconds = glfwGetTime ();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		 sprintf (tmp, "opengl @ fps: %.2f", fps);
		 glfwSetWindowTitle (window, tmp);
		 frame_count = 0;
	}
	frame_count++;
}

void print_shader_info_log (GLuint shader_index) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog (shader_index, max_length, &actual_length, log);
	printf ("shader info log for GL index %i:\n%s\n", shader_index, log);
	gl_log ("shader info log for GL index %i:\n%s\n", shader_index, log);
}

void _print_programme_info_log(unsigned int programmeShader)
{
	int max_lenght = 2048;
	int actual_lenght = 0;

	char log[2048];
	glGetProgramInfoLog( programmeShader, max_lenght, &actual_lenght, log);
	cout << "program info log for GL index: " << programmeShader << ":" << log << endl;
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

