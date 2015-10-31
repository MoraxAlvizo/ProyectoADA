#include "MathUtils.h"
#include "GLUtils.h" 
#include "GLCamera.h"
#include "GLProgram.h"
#include "GLObject.h"
#include "GLMesh.h"
#include "GLScene.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MESH_FILE "sphere.obj"
#define VERTEX_RED_SHADER_FILE "vsRed.glsl"
#define VERTEX_PURPLE_SHADER_FILE "vsPurple.glsl"
#define FRAGMENT_RED_SHADER_FILE "fsRed.glsl"
#define FRAGMENT_PURPLE_SHADER_FILE "fsPurple.glsl"
#define NUM_SPHERES 4


// a world position for each sphere in the scene
vector3 sphere_pos_wor[] = {
	vector3(-2.0, 0.0, 0.0),
	vector3(2.0, 0.0, 0.0),
	vector3(-2.0, 0.0, -2.0),
	vector3(1.5, 1.0, -1.0)
};

int main () {
/*--------------------------------START OPENGL--------------------------------*/
	assert (restart_gl_log ());
	// start GL context and O/S window using the GLFW helper library
	assert (start_gl ());
/*------------------------------CREATE GEOMETRY-------------------------------*/
	
	GLObject* sphere = new GLObject(MESH_FILE, 1);
	
/*-------------------------------CREATE PROGRAM-------------------------------*/
	
	GLProgram* programPurple = new GLProgram();
	GLProgram* programRed = new GLProgram();

	// Add shaders
	programPurple->addShader(new GLShader(FRAGMENT_PURPLE_SHADER_FILE,GL_FRAGMENT_SHADER ) );
	programPurple->addShader(new GLShader(VERTEX_PURPLE_SHADER_FILE,GL_VERTEX_SHADER));

	// create program
	programPurple->createProgram();
	programPurple->addVariable("model");
	programPurple->addVariable("view");
	programPurple->addVariable("proj");

	// Add shaders
	programRed->addShader(new GLShader(FRAGMENT_RED_SHADER_FILE,GL_FRAGMENT_SHADER ) );
	programRed->addShader(new GLShader(VERTEX_RED_SHADER_FILE,GL_VERTEX_SHADER));

	// create program
	programRed->createProgram();
	programRed->addVariable("model");
	programRed->addVariable("view");
	programRed->addVariable("proj");
	
/*-------------------------------CREATE CAMERA--------------------------------*/
	
	GLCamera camara((float)g_gl_width / (float)g_gl_height);
	camara.setPosition(vector3(0.0f, 0.0f, 5.0f));
	
/*---------------------------SET RENDERING DEFAULTS---------------------------*/
	
	glUseProgram (programPurple->getID());
	glUniformMatrix4fv (programPurple->getVariable("view") , 1, GL_FALSE, camara.getView().m);
	glUniformMatrix4fv (programPurple->getVariable("proj") , 1, GL_FALSE, camara.getProj().m);

	glUseProgram (programRed->getID());
	glUniformMatrix4fv (programRed->getVariable("view") , 1, GL_FALSE, camara.getView().m);
	glUniformMatrix4fv (programRed->getVariable("proj") , 1, GL_FALSE, camara.getProj().m);
	// Create scene
	GLScene* scene = new GLScene();

	//Create objects
	for (int i = 0; i < NUM_SPHERES; i++) 
	{
		if(i%2)
			scene->addMesh(new GLMesh(sphere, sphere_pos_wor[i], programPurple));
		else
			scene->addMesh(new GLMesh(sphere, sphere_pos_wor[i], programRed));

	}	
	
	glEnable (GL_DEPTH_TEST);			// enable depth-testing
	glDepthFunc (GL_LESS);				// depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE);			// cull face
	glCullFace (GL_BACK);				// cull back face
	glFrontFace (GL_CCW);				// set counter-clock-wise vertex order to mean the front
	glClearColor (0.2, 0.2, 0.2, 1.0);	// grey background to help spot mistakes
	glViewport (0, 0, g_gl_width, g_gl_height);
	
	vector3 position = sphere_pos_wor[0];
	bool insertar = false;

/*-------------------------------RENDERING LOOP-------------------------------*/
	while (!glfwWindowShouldClose (g_window)) 
	{
		// update timers
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		_update_fps_counter (g_window);
		
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Render Meshes
		scene->render();

		// update other events like input handling 
		glfwPollEvents ();
		
		// control keys
		bool cam_moved = false;
		vector3 move (0.0, 0.0, 0.0);

		if (insertar && glfwGetKey (g_window, GLFW_KEY_O) == GLFW_RELEASE) 
		{
			position += vector3(1.0,1.0,1.0);
			scene->addMesh(new GLMesh(sphere,position, programRed));
			insertar = false;
		}
		if (glfwGetKey (g_window, GLFW_KEY_O) == GLFW_PRESS) 
		{
			insertar = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_A)) 
		{
			move.v[0] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_D))
		{
			move.v[0] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_Q)) 
		{
			move.v[1] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_E)) 
		{
			move.v[1] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_W)) 
		{
			move.v[2] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_S)) 
		{
			move.v[2] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_LEFT)) 
		{
			cam_moved = true;
			camara.moveYawLeft(elapsed_seconds);
		}
		if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) 
		{
			cam_moved = true;
			camara.moveYawRight(elapsed_seconds);
		}
		if (glfwGetKey (g_window, GLFW_KEY_UP)) 
		{
			cam_moved = true;
			camara.movePitchUp(elapsed_seconds);
		}
		if (glfwGetKey (g_window, GLFW_KEY_DOWN)) 
		{
			cam_moved = true;
			camara.movePitchDown(elapsed_seconds);
		}
		if (glfwGetKey (g_window, GLFW_KEY_Z)) 
		{
			cam_moved = true;
			camara.rollLeft(elapsed_seconds);
		}
		if (glfwGetKey (g_window, GLFW_KEY_C))
		{
			cam_moved = true;
			camara.rollRight(elapsed_seconds);

		}
		// update view matrix
		if (cam_moved) 
		{
			camara.updatePosition(move);

			// Use the program and set the variable
			glUseProgram (programRed->getID());
			glUniformMatrix4fv (programRed->getVariable("view") , 1, GL_FALSE, camara.getView().m);
			
			// Use the program and set the variable
			glUseProgram (programPurple->getID());
			glUniformMatrix4fv (programPurple->getVariable("view") , 1, GL_FALSE, camara.getView().m);
			
		}
		
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) 
		{
			glfwSetWindowShouldClose(g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}