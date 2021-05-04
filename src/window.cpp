#pragma once
#include "glad/glad.h"
#include "window.h"
#include <iostream>

/*
	to explain of the configuration file work for the window
	inside the winConf vector data is alligner like that

	0: width
	1: height
	2: Fullscreen
	3: openGL major version
	4: openGL minor version
*/
GLFWwindow * nerv::createWindow(nerv::config* conf)
{
	//read config file
	int sector = nerv::findConfigSubsector(conf, "window");
	std::vector<int> winConf = nerv::readConfigSubsector(conf, sector);

	//set keybind global data
	sector = nerv::findConfigSubsector(conf, "key");
	gWindowsKeybind = nerv::readConfigSubsector(conf, sector);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set the openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, winConf[3]);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, winConf[4]);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//windows pointer that going to be returned
	GLFWwindow* win = nullptr;

	//check if fullscreen
	if (winConf[2])
		win = glfwCreateWindow(winConf[0], winConf[1], "Nerv: Neutral Environement for Ray Visialisation", glfwGetPrimaryMonitor(), NULL);
	else
		win = glfwCreateWindow(winConf[0], winConf[1], "Nerv: Neutral Environement for Ray Visialisation", NULL, NULL);

	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//setup window callback
	//TODO add size and framebuffer size callback
	glfwSetKeyCallback(win, windowKeyCallback);
	glfwSetFramebufferSizeCallback(win, windowframebufferSizeCallback);

	//make the windows the actual context
	glfwMakeContextCurrent(win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		exit(EXIT_FAILURE);

	//get windows framebuffer size and set the glviewport
	int width;
	int height;
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(1);

	return win;
}


/*
	this use a global buffer that hold evey keybind set in the config file
	this is setup on the glfw key value spreadsheet

	0: forward
	1: backward
	2: left
	3: right
	4: up
	5: down
	6: freeMouse
	7: closeWindow
*/
void nerv::windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == gWindowsKeybind[7] && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void nerv::windowframebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void nerv::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
}
void nerv::windowCloseCallback(GLFWwindow* window)
{

}
