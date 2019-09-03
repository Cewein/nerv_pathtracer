#include "windows.h"

void nerv::window::create(std::string name, int width, int height, bool fullscreen)
{
	if (!glfwInit())
	{
		logger.crash("GLFW", "Failed to init GLFW");
		exit(0);
	}

	logger.warning("GLFW", "Creating a window");

	logger.info("GLFW", fullscreen?"window full scren state : yes":"window full scren state : no");
	GLFWmonitor * monitor = NULL;
	if (fullscreen) monitor = glfwGetPrimaryMonitor();

	this->glfwDisplay = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);

	if (!this->glfwDisplay)
	{
		logger.crash("GLFW", "Failed to create a window");
		glfwTerminate();
		exit(0);
	}

	this->width = width;
	this->height = height;
	this->name = name;

	makeContext();
}

void nerv::window::makeContext()
{
	logger.warning("GLFW", "making context for the window");
	glfwMakeContextCurrent(this->glfwDisplay);
}

bool nerv::window::isOpen()
{
	return !glfwWindowShouldClose(this->glfwDisplay);
}

void nerv::window::close()
{
	logger.warning("GLFW", "Closing window");
	glfwTerminate();
}

void nerv::window::update()
{
	glfwSwapBuffers(glfwDisplay);
	glfwPollEvents();
}

GLFWwindow* nerv::window::display()
{
	return this->glfwDisplay;
}
