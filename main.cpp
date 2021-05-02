#include <iostream>
#include <glad/glad.h>

#include "src/window.h"
#include "src/shader.h"

int main()
{
	std::cout << "loading config\n";
	nerv::config conf = nerv::loadConfig("E:\\nerv_engine\\nerv_pathtracer\\config.ini");

	GLFWwindow* win = nerv::createWindow(&conf);

	while (!glfwWindowShouldClose(win))
	{
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}