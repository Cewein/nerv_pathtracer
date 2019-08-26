#include "windows.h"

void window::create()
{
	if (!glfwInit())
	{
		logger.crash("GLFW", "Failed to init GLFW");
		exit(0);
	}
	logger.warning("GLFW", "Creating a window");
	this->glfwDisplay = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!this->glfwDisplay)
	{
		logger.crash("GLFW", "Failed to create a window");
		glfwTerminate();
		exit(0);
	}

	makeContext();
}

void window::makeContext()
{
	logger.warning("GLFW", "making context for the window");
	glfwMakeContextCurrent(this->glfwDisplay);
}

bool window::isOpen()
{
	return !glfwWindowShouldClose(this->glfwDisplay);
}

void window::end()
{
	logger.info("GLFW", "Closing window");
	glfwTerminate();
}

GLFWwindow* window::display()
{
	return this->glfwDisplay;
}
