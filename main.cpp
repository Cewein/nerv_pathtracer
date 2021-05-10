#include <iostream>
#include <glad/glad.h>

#include "src/window.h"
#include "src/camera.h"
#include "src/shader.h"

int main()
{
	std::cout << "loading config\n";
	nerv::config conf = nerv::loadConfig("E:\\nerv_engine\\nerv_pathtracer\\config.ini");

	GLFWwindow* win = nerv::createWindow(&conf);
	nerv::camera cam = nerv::createCamera(&conf);

	nerv::shader mainShader("shader/basic.frag.glsl", "shader/static.vert.glsl");


	while (!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

         mainShader.use();

		 nerv::updateCamera(&cam, win);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}