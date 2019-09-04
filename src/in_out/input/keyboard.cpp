#include "keyboard.h"

//init the keyboard callback
void nerv::keyboard::init()
{
	logger.info("keyboard", "init callback");
	glfwSetKeyCallback(nerv::screen.display(), nerv::keyboard::keyboardCallBack);
}

//keyboard callback
void nerv::keyboard::keyboardCallBack(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(nerv::screen.display(), 1);
	if (keyIsPess(GLFW_KEY_A))
		if(keyIsPess(GLFW_KEY_B))
			glfwSetWindowShouldClose(nerv::screen.display(), 1);
}
