#pragma once

#include "../../../dependencies.h"

#define screen window::get()

class window {
public:
	int width;
	int height;

	static window& get() {
		static window instance;
		return instance;
	};

	void create();
	void makeContext();
	bool isOpen();
	void end();
	GLFWwindow* display();

private: 
	GLFWwindow* glfwDisplay;
	window() {}
};