#pragma once
#include <iostream>
#include <glad/glad.h>

#include "src/camera.h"
#include "src/window.h"
#include "src/shader.h"
#include "src/data.h"
#include "src/ui.h"
#include "src/bvh.h"

 
int main()
{
	std::cout << "loading config\n";
	nerv::config conf = nerv::loadConfig("E:\\nerv_engine\\nerv_pathtracer\\config.ini");

	GLFWwindow* win = nerv::createWindow(&conf);
	nerv::camera cam = nerv::createCamera(&conf);
	nerv::createUI(win); 

	  
	nerv::renderData render = {
		0,
		5,
		false, 
		0.0
	}; 

	nerv::shader mainShader("shader/raytraced.frag.glsl", "shader/static.vert.glsl");

	int width;
	int height;
	  
	glfwGetFramebufferSize(win, &width, &height);

	nerv::material* arrMat = nerv::genRandomMaterial(51); 

	std::vector<nerv::triangle> triangles = nerv::loadObj("E:\\nerv_engine\\nerv_pathtracer\\model\\bunny-heavy.obj");

	nerv::bvhNode* bvh = nerv::createNode(triangles, 0, triangles.size());
	int size = nerv::countNode(bvh);
	nerv::linearBvhNode * flatten = new nerv::linearBvhNode[size]; 
	 
	int offset = 0;
	
	nerv::flattenBVH(flatten, bvh, &offset, 0);

	size_t triangleBuffer = nerv::createBuffer(sizeof(nerv::triangle) * triangles.size(), triangles.data(), 0, GL_SHADER_STORAGE_BUFFER);
	size_t colorBuffer = nerv::createBuffer(sizeof(float) * 4 * width * height, nullptr, 1, GL_SHADER_STORAGE_BUFFER);
	size_t bvhBuffer = nerv::createBuffer(sizeof(nerv::linearBvhNode) * size, flatten, 2, GL_SHADER_STORAGE_BUFFER);

	size_t materialBuffer = nerv::createBuffer(sizeof(nerv::material) * 51, arrMat, 3, GL_SHADER_STORAGE_BUFFER);

	nerv::texture background = nerv::loadImage("resources/evening_road_01.jpg");

	while (!glfwWindowShouldClose(win))
	{
		//pool event the render
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		//update
		if (!nerv::gUsingMenu)
			render.isMoving = nerv::updateCamera(&cam, win);

		//send info to shader
		nerv::sendInfo(&cam, &render, win); 

		mainShader.activateImage(&background, "background", 0);
		
		//render
        mainShader.use();
		nerv::displayUI(win, &render, &cam);


		
		glfwSwapBuffers(win);

		
	}

	//nerv::closeUI();

	glfwDestroyWindow(win);
	glfwTerminate();

	delete arrMat; 
	//delete arrSphere;
	delete flatten;

	return EXIT_SUCCESS;
}