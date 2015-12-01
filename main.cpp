#include "MathUtils.h"
#include "GLUtils.h" 
#include "GLCamera.h"
#include "GLProgram.h"
#include "GLObject.h"
#include "GLMesh.h"
#include "GLScene.h"
#include "GLOctree.h"
#include <GLFW/glfw3.h> 
#include <GL/glew.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MESH_FILE "Meshes/sphere.obj"
#define VERTEX_RED_SHADER_FILE "Shaders/vsRed.glsl"
#define VERTEX_PURPLE_SHADER_FILE "Shaders/vsPurple.glsl"
#define FRAGMENT_RED_SHADER_FILE "Shaders/fsRed.glsl"
#define FRAGMENT_PURPLE_SHADER_FILE "Shaders/fsPurple.glsl"

Octree* _octree; //An octree with all af the balls
float randomPos(){
	float pos = 0;
	pos = (rand() % (int)BOX_SIZE) / 2  ;
	return pos;
}

int main() {
	/*--------------------------------START OPENGL--------------------------------*/
	assert(restart_gl_log());
	// start GL context and O/S window using the GLFW helper library
	assert(start_gl());
	/*------------------------------CREATE GEOMETRY-------------------------------*/

	GLObject* sphere = new GLObject(MESH_FILE, 1);

	/*------------------------------ CREATE GLText -------------------------------*/
	//GLText* text = new GLText();

	/*-------------------------------CREATE PROGRAM-------------------------------*/

	GLProgram* programPurple = new GLProgram();
	GLProgram* programRed = new GLProgram();

	// Add shaders
	//programPurple->addShader(new GLShader(FRAGMENT_PURPLE_SHADER_FILE,GL_FRAGMENT_SHADER ) );
	programPurple->addShader(new GLShader(FRAGMENT_PURPLE_SHADER_FILE, GL_FRAGMENT_SHADER));
	programPurple->addShader(new GLShader(VERTEX_RED_SHADER_FILE, GL_VERTEX_SHADER));

	// create program
	programPurple->createProgram();
	programPurple->addVariable("model");
	programPurple->addVariable("view");
	programPurple->addVariable("proj");

	// Add shaders
	programRed->addShader(new GLShader(FRAGMENT_RED_SHADER_FILE, GL_FRAGMENT_SHADER));
	programRed->addShader(new GLShader(VERTEX_RED_SHADER_FILE, GL_VERTEX_SHADER));

	// create program
	programRed->createProgram();
	programRed->addVariable("model");
	programRed->addVariable("view");
	programRed->addVariable("proj");

	/*-------------------------------Create Octree ------------------------------*/

	_octree = new Octree(vector3(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2),
		vector3(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2), 1);

	/*-------------------------------CREATE CAMERA--------------------------------*/
	int limit = 3;
	GLCamera camara((float)g_gl_width / (float)g_gl_height);
	camara.look_at(vector3((float)30.0, (float)10.0, (float)30.0), vector3(5.0, 5.0, 5.0), vector3(5.0, 5.0, 5.0));
	//camara.setPosition(vector3((float)15.0, (float)5.0, (float)15.0));

	/*---------------------------SET RENDERING DEFAULTS---------------------------*/

	glUseProgram(programPurple->getID());
	glUniformMatrix4fv(programPurple->getVariable("view"), 1, GL_FALSE, camara.getView().m);
	glUniformMatrix4fv(programPurple->getVariable("proj"), 1, GL_FALSE, camara.getProj().m);

	glUseProgram(programRed->getID());
	glUniformMatrix4fv(programRed->getVariable("view"), 1, GL_FALSE, camara.getView().m);
	glUniformMatrix4fv(programRed->getVariable("proj"), 1, GL_FALSE, camara.getProj().m);


	// Create scene
	GLScene* scene = new GLScene();

	// Initial position
	vector3 sphere_pos = vector3(0.0, 0.0, 0.0);
	int i = 0;
	//Create objects
	for (int x = 0; x < limit; x += 2)
	for (int y = 0; y < limit; y += 2)
	for (int z = 0; z < limit; z += 2)
	{
		GLMesh* mesh = new GLMesh(sphere, vector3(x, y, z), ++i % 2 ? programRed : programPurple);
		scene->addMesh(mesh);
		_octree->add(mesh);

	}
	scene->printProperties();

	glEnable(GL_DEPTH_TEST);			// enable depth-testing
	glDepthFunc(GL_LESS);				// depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);			// cull face
	glCullFace(GL_BACK);				// cull back face
	glFrontFace(GL_CCW);				// set counter-clock-wise vertex order to mean the front
	glClearColor(0.2, 0.2, 0.2, 1.0);	// grey background to help spot mistakes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, g_gl_width, g_gl_height);

	vector3 position = vector3(limit, limit, limit);
	bool insertar = false;



	/*-------------------------------RENDERING LOOP-------------------------------*/
	while (!glfwWindowShouldClose(g_window))
	{
		// update timers
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		_update_fps_counter(g_window);

		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Meshes
		scene->render();

		float sx = 2.0 / g_gl_width;
		float sy = 2.0 / g_gl_height;


		// Render text
		//text->render("This is sample text", 25.0f, 25.0f, 1.0f, vector3(0.5, 0.8f, 0.2f));
		//text.render("The Quick Brown Fox Jumps Over The Lazy Dog",
		//      -1 + 8 * sx,   1 - 50 * sy,    sx, sy);

		// update other events like input handling 
		glfwPollEvents();

		// control keys
		bool cam_moved = false;
		vector3 move(0.0, 0.0, 0.0);
		
		scene->moveAll(vector3(0.0, elapsed_seconds, 0), _octree);
		//_octree->printCHildren(0, _octree);
		if (insertar && glfwGetKey(g_window, GLFW_KEY_O) == GLFW_RELEASE)
		{
			for (int i = 0; i < 20; i++)
			{
				GLMesh * mesh = new GLMesh(sphere, vector3(randomPos(), randomPos(), randomPos()), i % 2 ? programRed : programPurple);
				scene->addMesh(mesh);
				_octree->add(mesh);

				
			}
			printf("Cantidad de pelotas: %u\n", scene->getNumMeshes());
			insertar = false;
		}
		if (glfwGetKey(g_window, GLFW_KEY_O) == GLFW_PRESS)
		{
			insertar = true;
		}

		if (glfwGetKey(g_window, GLFW_KEY_A))
		{
			move.v[0] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_D))
		{
			move.v[0] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_Q))
		{
			move.v[1] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_E))
		{
			move.v[1] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_W))
		{
			move.v[2] -= camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_S))
		{
			move.v[2] += camara.getSpeed() * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(g_window, GLFW_KEY_LEFT))
		{
			cam_moved = true;
			camara.moveYawLeft(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_RIGHT))
		{
			cam_moved = true;
			camara.moveYawRight(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_UP))
		{
			cam_moved = true;
			camara.movePitchUp(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_DOWN))
		{
			cam_moved = true;
			camara.movePitchDown(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_Z))
		{
			cam_moved = true;
			camara.rollLeft(elapsed_seconds);
		}
		if (glfwGetKey(g_window, GLFW_KEY_C))
		{
			cam_moved = true;
			camara.rollRight(elapsed_seconds);

		}
		// update view matrix
		if (cam_moved)
		{
			camara.updatePosition(move);

			// Use the program and set the variable
			glUseProgram(programRed->getID());
			glUniformMatrix4fv(programRed->getVariable("view"), 1, GL_FALSE, camara.getView().m);

			// Use the program and set the variable
			glUseProgram(programPurple->getID());
			glUniformMatrix4fv(programPurple->getVariable("view"), 1, GL_FALSE, camara.getView().m);

		}

		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
