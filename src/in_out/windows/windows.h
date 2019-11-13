#pragma once
#include <iostream>
#include <string>


#include "../../../dependencies.h"
#define WINDOW_GLFW_DISPLAY nerv::window::get().display() 


namespace nerv
{
	class window {
	public:
		int width;
		int height;
		std::string name;
		

		//singelton for the windows so it can be access everywhere
		static window& get() {
			static window instance;
			return instance;
		};

		void create(std::string name, int width, int height, bool fullscreen = false);

		//as of now it doesn't work
		void framebufferSizeCallback(GLFWwindow * win, int width, int height);
		bool isOpen();
		void close();
		void update();

		//time function
		void calculateDeltaTime();
		inline float getDeltaTime() { return deltaTime; };

		//actual GLFW display, everything is in it, see GLFW docs for more info
		GLFWwindow* display();

	private: 
		float deltaTime;
		float lastFrame;
		GLFWwindow* glfwDisplay;
		window() {}
	};
}