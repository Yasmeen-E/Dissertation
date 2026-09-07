#pragma once

#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include <memory>
//Eval
#include <chrono>
//GLEW
#include  <GL/glew.h> 
// GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "loadPipe.h"
#include "ImguiLayer.h"
#include "imguiSetting.hpp"

//Camera 
#include "ArcballCamera.hpp"
#include "FlythroughCamera.hpp"
#include "InputManager.hpp"

#include "OcclusionMap.hpp"
#include "Scene.hpp"
#include "Texture3D.hpp"

#define WIDTH 1920
#define HEIGHT 1080


#define OC_MAP_WIDTH 2048
#define OC_MAP_HEIGHT 2048

using namespace glm;


struct SceneUBO {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 projView;
    glm::vec4 camPos;
};


void updateUscene(SceneUBO &sceneUniform, Camera *camera);


void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac needed 
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
	glfwSetWindowSizeCallback(window, SizeCallback);
	glfwSwapInterval(0);//v-sync

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//set up Imgui
	ImGuiLayer imguiLayer;
    imguiLayer.Init(window,"#version 150");

	Settings setting;

	//background
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// shaders 
	GLuint sceneID = LoadShaders( "scene.vert", "scene.frag" );
	GLuint snowID = LoadShaders( "snow.vert", "snow.frag" );
	GLuint occludeID = LoadShaders( "shadow.vert", "shadow.frag" );

	ShadowStruct occlude = setup_shadowmap(OC_MAP_WIDTH, OC_MAP_HEIGHT);
	
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

	//add scene ubo to both shaders
	GLuint blockIndex1 = glGetUniformBlockIndex(snowID, "UScene");
	glUniformBlockBinding(snowID, blockIndex1, 0);

	GLuint blockIndex2 = glGetUniformBlockIndex(sceneID, "UScene");
	glUniformBlockBinding(sceneID, blockIndex2, 0);

	//setup 3D noise texture
	GLuint texnoise3d= createPerlinTexture3D(64);
	vec3 LightDirection = vec3(0.0f, -1.0f, 0.0f);

    //initialise 
	Scene scene;
	// auto lamp = std::make_shared<Model>("OBJs/SpotLamp/Lamp.obj", "OBJs/SpotLamp/render_d.png");
	// glm::mat4 sl = glm::mat4(1.0f);

	// sl = glm::translate(sl, glm::vec3(0.f, 0.f, 50.f)); 
	// sl = glm::scale(sl, glm::vec3(15.f, 15.f, 15.f)); 
	// lamp->transform = sl;

	// scene.addModel(lamp);

	// auto house = std::make_shared<Model>("OBJs/Haus.obj", "OBJs/Haus.jpg");
	// glm::mat4 l = glm::mat4(1.0f);
	// house->transform = glm::scale(l, glm::vec3(0.25f, 0.25f, 0.25f));
	// scene.addModel(house);
	// //need to add scale to shaders

	// auto bench = std::make_shared<Model>("OBJs/Crate.obj", "OBJs/Crate.png");
	// glm::mat4 b = glm::mat4(1.0f);
	// b = glm::translate(b, glm::vec3(30.f, 0.f, 30.f)); 
	// bench->transform = glm::scale(b, glm::vec3(2.f, 2.f, 2.f));
	// scene.addModel(bench);


	// auto ground = std::make_shared<GroundPlane>(120.0f, 20, "OBJs/grass.jpg");

	// auto cruiser = std::make_shared<Model>("OBJs/test/cruiser/cruiser.obj", "OBJs/test/cruiser/cruiser.bmp");
	// scene.addModel(cruiser);

	// shadow maps are hella big and pcf pretty much gets rid of everythign something to do with scale ig
	auto dragon = std::make_shared<Model>("OBJs/test/dragon.obj", "OBJs/test/f16/f1s.bmp");
	scene.addModel(dragon);

	// auto bunny = std::make_shared<Model>("OBJs/test/bunny.obj", "OBJs/test/f16/f1s.bmp");
	// scene.addModel(bunny);

	// auto suzanne = std::make_shared<Model>("OBJs/test/suzanne/suzanne.obj", "OBJs/test/f16/f1s.bmp");
	// scene.addModel(suzanne);

	//auto f16 = std::make_shared<Model>("OBJs/test/f16/f16.obj", "OBJs/test/f16/f16s.bmp");
	//scene.addModel(f16);

	// Move it, scale it etc
	//ground->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, 0.f));
	//scene.addGround(ground);


	printf("models loaded\n"); 

	float lastFrame = 0.0f;

	do{

		auto frameStart = std::chrono::high_resolution_clock::now();
		

		int fbWidth, fbHeight;
    	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		float currentFrame = glfwGetTime();
		float deltaTime    = currentFrame - lastFrame;
		lastFrame          = currentFrame;

		flythrough.update(deltaTime);
		arcball.update(deltaTime);

		glm::vec3 snowDirection = glm::vec3 (0.f , -1.f, 0.f);

		if (setting.wind){
			float angleRad = glm::radians(setting.windAngle);
			snowDirection = glm::normalize(glm::vec3(sin(angleRad), -1.0f, cos(angleRad)));
		}
		//preprocessing (for now) (shadow mapping)
		glViewport(0, 0, OC_MAP_WIDTH, OC_MAP_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, occlude.FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(occludeID);
		glUniformMatrix4fv(glGetUniformLocation(occludeID, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(getOrtho(snowDirection)));

		scene.drawOcclude(occludeID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//scene will be drawn twice
		// once normally
		// then again displaced 
		// draw reset of scene

		glViewport(0, 0, fbWidth, fbHeight);
		static const GLfloat bgd[] = { .8f, .8f, .8f, 1.f };
		glClearBufferfv(GL_COLOR, 0, bgd);
		glClear(GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//scene uniform 
		SceneUBO SceneUniform;
		updateUscene(SceneUniform,input.getActiveCamera());
		glBindBuffer(GL_UNIFORM_BUFFER, sceneubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneUBO), &SceneUniform);


		//draw scene no snow
		glUseProgram(sceneID);
		glUniform3fv(glGetUniformLocation(sceneID, "lightDirection"), 1, glm::value_ptr(LightDirection));
		//draw scene
		scene.draw(sceneID);

		if (setting.snow)
		{
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//draw snow with displaces vertex
			glUseProgram(snowID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, occlude.Texture);
			glUniformMatrix4fv(glGetUniformLocation(snowID, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(getOrtho(snowDirection)));
			glUniform3f(glGetUniformLocation(snowID, "snowDirection"),0.f , -1.f, 0.f);
			glUniform3fv(glGetUniformLocation(snowID, "lightDirection"), 1, glm::value_ptr(LightDirection));
			glUniform1f(glGetUniformLocation(snowID, "t"), setting.time);

			//bind 3D Perlin Noise texture
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_3D, texnoise3d);
			glUniform1i(glGetUniformLocation(snowID, "NoiseSampler"), 2);

			//draw scene
			scene.draw(snowID);
			glDisable(GL_BLEND);
		}


		//draw imgui
		imguiLayer.BeginFrame();
        imguiLayer.DrawUI(setting);
        imguiLayer.EndFrame();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


		auto frameEnd = std::chrono::high_resolution_clock::now();

		double frameTime = std::chrono::duration<double, std::milli>(
			frameEnd - frameStart
		).count();

    	double fps = 1000.0 / frameTime;

		//print the fps every second
		static double fpsTimer = 0.0;
		static int frameCount = 0;

		fpsTimer += frameTime;
		frameCount++;

		if (fpsTimer >= 1000.0) {
			double averageFPS = frameCount / (fpsTimer / 1000.0);

			std::cout << "FPS: " << averageFPS
					<< " | Frame time: " << fpsTimer / frameCount
					<< " ms\n";

			fpsTimer = 0.0;
			frameCount = 0;
		}
	


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


	//clean	
	imguiLayer.Shutdown();
	glDeleteTextures(1,&texnoise3d);
	glDeleteProgram(sceneID);
	glDeleteProgram(snowID);
	glDeleteProgram(occludeID);
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

