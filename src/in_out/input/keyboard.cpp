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
	if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(WINDOW_GLFW_DISPLAY, 1);

	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
	{
		camera::focusing = !camera::focusing; // better way than static ?

		if(camera::focusing)
			glfwSetInputMode(WINDOW_GLFW_DISPLAY, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(WINDOW_GLFW_DISPLAY, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

//Input a Camera and get a FPS gamera
void nerv::keyboard::updateCameraKeyboard(nerv::camera * camera)
{
	if (camera::focusing)
	{
		if (nerv::camera::raytraced)
		{
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_W) == GLFW_PRESS)
			{
				camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->up;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_S) == GLFW_PRESS)
			{
				camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->up;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->front;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->front;
				camera->isMoving = true;
			}
		}
		else
		{
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_W) == GLFW_PRESS)
			{
				camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->front;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_S) == GLFW_PRESS)
			{
				camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->front;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * camera->up;
				camera->isMoving = true;
			}
			if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * camera->up;
				camera->isMoving = true;
			}
		}
		if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera->transform->positionVec += nerv::window::get().getDeltaTime() * camera->speed * glm::normalize(glm::cross(camera->front, camera->up));
			camera->isMoving = true;
		}
		if (glfwGetKey(WINDOW_GLFW_DISPLAY, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera->transform->positionVec -= nerv::window::get().getDeltaTime() * camera->speed * glm::normalize(glm::cross(camera->front, camera->up));
			camera->isMoving = true;
		}
	}
	
}

void nerv::mouse::updateCameraMouse(nerv::camera * camera)
{
	if (camera::focusing)
	{
		double xpos, ypos;
		glfwGetCursorPos(WINDOW_GLFW_DISPLAY, &xpos, &ypos);

		if (xpos != camera->lastX || ypos != camera->lastY)
		{
			camera->isMoving = true;

			float xoffset = xpos - camera->lastX;
			float yoffset = camera->lastY - ypos;

			camera->lastX = xpos;
			camera->lastY = ypos;

			xoffset *= camera->sensitivity;
			yoffset *= camera->sensitivity;

			camera->yaw += xoffset;
			camera->pitch += yoffset;
			
			if (camera->pitch > (89.9f - 90.f) && nerv::camera::raytraced)
				camera->pitch = (89.9f - 90.f);
			else if (camera->pitch > 89.9f && !nerv::camera::raytraced)
				camera->pitch = 89.9f;
			if (camera->pitch < (-89.9f - 90.f) && nerv::camera::raytraced)
				camera->pitch = (-89.9f - 90.f);
			else if (camera->pitch < -89.9f && !nerv::camera::raytraced)
				camera->pitch = -89.9f;

			glm::vec3 front;

			front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
			front.y = sin(glm::radians(camera->pitch));
			front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

			camera->front = glm::normalize(front);
		}
	}
}


