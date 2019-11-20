#include "keyboard.h"

//init the keyboard callback
void nerv::keyboard::init()
{
	logger.info("keyboard", "init callback");
	glfwSetKeyCallback(nerv::window::get().display(), nerv::keyboard::keyboardCallBack);
}

//keyboard callback
void nerv::keyboard::keyboardCallBack(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(nerv::window::get().display(), 1);
	if (keyIsPess(GLFW_KEY_A))
		if(keyIsPess(GLFW_KEY_B))
			glfwSetWindowShouldClose(nerv::window::get().display(), 1);
}

void nerv::keyboard::updateCameraKeyboard(nerv::camera * camera)
{
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_W) == GLFW_PRESS)
		camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * glm::vec3(0.f, 0.f, 1.f);
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_S) == GLFW_PRESS)
		camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * glm::vec3(0.f, 0.f, 1.f);
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_A) == GLFW_PRESS)
		camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * glm::vec3(1.f, 0.f, 0.f);
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_D) == GLFW_PRESS)
		camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * glm::vec3(1.f, 0.f, 0.f);
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_Q) == GLFW_PRESS)
		camera->transform->rotateX(10 * nerv::window::get().getDeltaTime() + glm::degrees(camera->transform->rotateVec.x));
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_E) == GLFW_PRESS)
		camera->transform->rotateX(-10 * nerv::window::get().getDeltaTime() + glm::degrees(camera->transform->rotateVec.x));
}
