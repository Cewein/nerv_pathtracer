#pragma once
#include "../../../dependencies.h"
#include "../windows/windows.h"

namespace nerv {
	namespace keyboard {
		void init();
		void keyboardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);
	};
}