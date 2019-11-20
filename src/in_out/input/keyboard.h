#pragma once
#include <iostream>
#include <string>

#include "../../../dependencies.h"
#include "../windows/windows.h"
#include "../../engine/object/camera.h"

#define keyIsPess(a) glfwGetKey(window, a) == GLFW_PRESS

namespace nerv {
	namespace keyboard {
		void init();
		void keyboardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);

		void updateCameraKeyboard(nerv::camera * cam);
	};

	namespace mouse {
		void updateCameraMouse();
	}
}