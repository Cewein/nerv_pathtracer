#pragma once
#include <iostream>
#include <string>

#include "../../../dependencies.h"
#include "../windows/windows.h"

#define keyIsPess(a) glfwGetKey(window, a) == GLFW_PRESS

namespace nerv {
	namespace keyboard {
		void init();
		void keyboardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);
	};
}