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
	nerv::sphere * arrSphere = nerv::genRandomSphere(520,50);

	std::vector<nerv::triangle> triangles = nerv::loadObj("E:\\nerv_engine\\nerv_pathtracer\\model\\rabbit.obj");

	nerv::bvhNode* bvh = nerv::createNode(triangles, 0, triangles.size());
	std::vector<nerv::linearBvhNode> flatten = std::vector<nerv::linearBvhNode>(nerv::countNode(bvh));

	int offset = 0;
	
	//need to fix that data assignement
	nerv::flattenBVH(flatten, bvh, &offset, 0);

	size_t triangleBuffer = nerv::createBuffer(sizeof(nerv::triangle) * triangles.size(), triangles.data(), 0, GL_SHADER_STORAGE_BUFFER);
	size_t colorBuffer = nerv::createBuffer(sizeof(float) * 4 * width * height, nullptr, 1, GL_SHADER_STORAGE_BUFFER);
	size_t bvhBuffer = nerv::createBuffer(sizeof(nerv::linearBvhNode) * flatten.size(), flatten.data(), 2, GL_SHADER_STORAGE_BUFFER);

	size_t materialBuffer = nerv::createBuffer(sizeof(nerv::material) * 51, arrMat, 3, GL_SHADER_STORAGE_BUFFER);
	size_t sphereBuffer = nerv::createBuffer(sizeof(nerv::sphere) * 520, arrSphere, 4, GL_SHADER_STORAGE_BUFFER);



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

		mainShader.setInt("nbSphere", 520);
		
		//render
        mainShader.use();
		nerv::displayUI(&render, &cam);


		
		glfwSwapBuffers(win);

		
	}

	//nerv::closeUI();

	glfwDestroyWindow(win);
	glfwTerminate();

	delete arrMat;
	delete arrSphere;

	return EXIT_SUCCESS;
}