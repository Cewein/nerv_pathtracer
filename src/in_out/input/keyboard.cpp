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

//Input a Camera and get a FPS gamera
void nerv::keyboard::updateCameraKeyboard(nerv::camera * camera)
{
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_W) == GLFW_PRESS)
		camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->front;
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_S) == GLFW_PRESS)
		camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->front;
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_A) == GLFW_PRESS)
		camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * glm::normalize(glm::cross(camera->front, camera->up));
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_D) == GLFW_PRESS)
		camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * glm::normalize(glm::cross(camera->front, camera->up));
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->up;
	if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->up;


	//logger.info("input", glm::to_string(camera->transform->positionVec));
}

void nerv::mouse::updateCameraMouse(nerv::camera * camera)
{
	double xpos, ypos;
	glfwGetCursorPos(nerv::window::get().display(), &xpos, &ypos);

	if (xpos != camera->lastX || ypos != camera->lastY)
	{


		float xoffset = xpos - camera->lastX;
		float yoffset = camera->lastY - ypos;

		camera->lastX = xpos;
		camera->lastY = ypos;

		xoffset *= camera->sensitivity;
		yoffset *= camera->sensitivity;

		camera->yaw += xoffset;
		camera->pitch += yoffset;

		if (camera->pitch > 89.9f)
			camera->pitch = 89.9f;
		if (camera->pitch < -89.9f)
			camera->pitch = -89.9f;

		glm::vec3 front;
		front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
		front.y = sin(glm::radians(camera->pitch));
		front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
		camera->front = glm::normalize(front);
	}
}
