#pragma once
#include "config.h"

#include <glfw/glfw3.h>
#include <vector>


namespace nerv {
	//create a window with from a given config file
	GLFWwindow * createWindow(nerv::config* conf);

	//gobal vector that hold keybind see windowKeyCallback for more info
	inline std::vector<int> gWindowsKeybind;
	
	//window callback function
	void windowSizeCallback(GLFWwindow* window, int width, int height);
	void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void windowCloseCallback(GLFWwindow* window);
	void windowframebufferSizeCallback(GLFWwindow* window, int width, int height);
}
