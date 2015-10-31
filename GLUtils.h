#ifndef _GL_UTILS_H_
#define _GL_UTILS_H_

#include <stdarg.h> // used by log functions to have variable number of args
#include <iostream>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

using namespace std;


/*------------------------------GLOBAL VARIABLES------------------------------*/
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;
/*--------------------------------LOG FUNCTIONS-------------------------------*/
bool restart_gl_log ();
bool gl_log (const char* message, ...);
/* same as gl_log except also prints to stderr */
bool gl_log_err (const char* message, ...);
const char* GL_type_to_string (unsigned int type);
/*--------------------------------GLFW3 and GLEW------------------------------*/
bool start_gl ();
void glfw_error_callback (int error, const char* description);
void glfw_window_size_callback (GLFWwindow* window, int width, int height);
void _update_fps_counter (GLFWwindow* window);
/*-----------------------------------SHADERS----------------------------------*/
void _print_programme_info_log(unsigned int programmeShader);
#endif
