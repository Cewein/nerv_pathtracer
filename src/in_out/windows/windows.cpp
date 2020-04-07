#pragma once
#include <glad/glad.h>
#include "windows.h"

void nerv::window::create(std::string name, int width, int height, bool fullscreen)
{
	if (!glfwInit())
	{
		logger.crash("GLFW", "Failed to init GLFW");
		exit(0);
	}

	logger.warning("GLFW", "Creating a window");

	//TODO make this link with the config file for openGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	logger.info("GLFW", fullscreen?"window full scren state : TRUE":"window full scren state : FALSE");
	GLFWmonitor * monitor = NULL;
	if (fullscreen) monitor = glfwGetPrimaryMonitor();

	this->glfwDisplay = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);

	if (!this->glfwDisplay)
	{
		logger.crash("GLFW", "Failed to create a window");
		glfwTerminate();
		exit(0);
	}

	logger.warning("GLFW", "making context for the window");
	glfwMakeContextCurrent(this->glfwDisplay);

	logger.warning("GLAD", "GLAD is initializing");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger.crash("GLAD", "GLAD did not initialize");
		exit(EXIT_FAILURE);
	}

	this->width = width;
	this->height = height;
	this->name = name;

	logger.warning("GLFW", "Setting view port for openGL\n");
	glViewport(0, 0, width, height);

	this->deltaTime = 0.0f;
	this->lastFrame = 0.0f;

	//todo fix the callback
	//glfwSetFramebufferSizeCallback(this->display, this->framebufferSizeCallback);

	glfwSetInputMode(this->glfwDisplay, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
}

void nerv::window::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
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
	calculateDeltaTime();
	glfwSwapBuffers(this->glfwDisplay);
	glfwPollEvents();
}

void nerv::window::calculateDeltaTime()
{
	float currentFrame = glfwGetTime();
	this->deltaTime = currentFrame - lastFrame;
	this->lastFrame = currentFrame;
}

GLFWwindow* nerv::window::display()
{
	return this->glfwDisplay;
}
