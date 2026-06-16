#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include  <GL/glew.h> 

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "loadPipe.h"


//Camera things
#include "ArcballCamera.hpp"
#include "FlythroughCamera.hpp"
#include "InputManager.hpp"

#define WIDTH 1920
#define HEIGHT 1080

using namespace glm;


struct SceneUBO {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 projView;
    glm::vec4 camPos;
};


void updateUscene(SceneUBO &sceneUniform, Camera *camera);

int main( void )
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WIDTH, HEIGHT, "Main", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//background
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "tri.vert", "tri.frag" );
	
	//set up camera
	FlythroughCamera flythrough(WIDTH, HEIGHT);
	ArcballCamera    arcball(WIDTH, HEIGHT, glm::vec3(0.0f));
	InputManager     input;

	input.setFlythroughCamera(&flythrough);
	input.setArcballCamera(&arcball);
	input.init(window);

	//setup SceneUniforms
	GLuint sceneubo;
	glGenBuffers(1, &sceneubo);
	glBindBuffer(GL_UNIFORM_BUFFER, sceneubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneUBO), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, sceneubo); 
	GLuint blockIndex = glGetUniformBlockIndex(programID, "UScene");
	glUniformBlockBinding(programID, blockIndex, 0);



	//initialise 
	
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	float lastFrame = 0.0f;

	do{
		
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		float currentFrame = glfwGetTime();
		float deltaTime    = currentFrame - lastFrame;
		lastFrame          = currentFrame;

		flythrough.update(deltaTime);
		arcball.update(deltaTime);

		//scene uniform 
		SceneUBO SceneUniform;
		updateUscene(SceneUniform,input.getActiveCamera());

		glBindBuffer(GL_UNIFORM_BUFFER, sceneubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneUBO), &SceneUniform);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);


	glfwTerminate();

	return 0;
}


void updateUscene(SceneUBO &sceneUniform, Camera *camera)
{

	sceneUniform.view       = camera->getView();
    sceneUniform.projection = camera->getProj();
    sceneUniform.projView   = sceneUniform.projection * sceneUniform.view;
    sceneUniform.camPos     = glm::vec4(camera->getPosition(), 1.0f);

}

