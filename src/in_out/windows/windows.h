#pragma once
#include "../../../dependencies.h"

#define screen window::get()

class window {
public:
	int width;
	int height;
	std::string name;

	static window& get() {
		static window instance;
		return instance;
	};

	void create(std::string name, int width, int height, bool fullscreen = false);
	void makeContext();
	bool isOpen();
	void close();
	void update();
	GLFWwindow* display();

private: 
	GLFWwindow* glfwDisplay;
	window() {}
};