#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
#include<glm\vec3.hpp>
#include<glm\vec4.hpp>
#include<glm\gtc\matrix_transform.hpp>

using namespace glm;

#pragma warning(disable : 4996)
#define GL_LOG_FILE "gl.log"
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

using namespace std;

void glfw_window_size_callback(GLFWwindow* window, int width, int height);
bool restarGlLog();
bool glLog( string message, string filename, int line);
void glfw_error_callback( int error,const char* descripcion);
void _update_fps_counter(GLFWwindow* window);
void _print_shader_info_log(unsigned int shaderIndex);
void _print_programme_info_log(unsigned int programmeShader);
void log_gl_params () ;
const char* GL_type_to_string (unsigned int type);
void mouse_click_callback (int b, int s, int mouse_x, int mouse_y);

#endif