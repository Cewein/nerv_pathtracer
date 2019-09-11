#pragma once
#include <iostream>
#include <string>


#include "../../../dependencies.h"

namespace nerv
{
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

		void framebufferSizeCallback(GLFWwindow * win, int width, int height);
		bool isOpen();
		void close();
		void update();

		GLFWwindow* display();

	private: 
		GLFWwindow* glfwDisplay;
		window() {}
	};
}