#pragma once
#include "glad/glad.h"

#include "camera.h"


/*
	read the config file and look at the subsecor for
	camera option, we provide the follow option array
	description:

	0: speed
	1: sensivity
	2: fov
	3: aperture
	4: focus distance

*/
nerv::camera nerv::createCamera(nerv::config* conf)
{
	//read config file
	int sector = nerv::findConfigSubsector(conf, "camera");
	std::vector<int> camConf = nerv::readConfigSubsector(conf, sector);

	//see window.cpp
	sector = nerv::findConfigSubsector(conf, "window");
	std::vector<int> winConf = nerv::readConfigSubsector(conf, sector);

	camera cam =
	{
		//position value
		glm::vec3(0.0),
		glm::mat4(1.0f),
		glm::vec3(0.0),
		glm::vec3(0.0),
		glm::vec3(0.0),
		0.0,
		-90.0,

		//controle value
		camConf[0] / 10.0,
		camConf[1] / 100.0,
		winConf[0] / 2,
		winConf[1] / 2,
		0.0,

		//lens value
		camConf[2],
		camConf[3],
		camConf[4]
	};
	
	return cam;
}

void nerv::updateFPSView(camera* cam)
{
	glm::vec3 eye = cam->position;

	float cosPitch = cosf(glm::radians(cam->pitch));
	float sinPitch = sinf(glm::radians(cam->pitch));
	float cosYaw = cosf(-glm::radians(cam->yaw));
	float sinYaw = sinf(-glm::radians(cam->yaw));

	glm::vec3 right = { cosYaw, 0, -sinYaw };
	glm::vec3 up = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	glm::vec3 front = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	cam->right = glm::normalize(right);
	cam->up = glm::normalize(up);
	cam->front = glm::normalize(front);

	cam->view = {
		glm::vec4(right.x,            up.x,            front.x,      0),
		glm::vec4(right.y,            up.y,            front.y,      0),
		glm::vec4(right.z,            up.z,            front.z,      0),
		glm::vec4(-glm::dot(right, eye), -glm::dot(up, eye), -glm::dot(front, eye), 1)
	};
}

bool nerv::updateCamera(camera* cam, GLFWwindow* win)
{
	bool moving = false;
	float deltaTime = cam->LastFrameTime - glfwGetTime();
	cam->LastFrameTime = glfwGetTime();
	
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam->position -= deltaTime * cam->speed * cam->up;
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam->position += deltaTime * cam->speed * cam->up;
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam->position -= deltaTime  * cam->speed * cam->front;
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cam->position += deltaTime * cam->speed * cam->front;
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam->position += deltaTime * cam->speed * cam->right;
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam->position -= deltaTime * cam->speed * cam->right;
		moving = true;
	}

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if (xpos != cam->lastX || ypos != cam->lastY)
	{
		float xoffset = xpos - cam->lastX;
		float yoffset = cam->lastY - ypos;

		cam->lastX = xpos;
		cam->lastY = ypos;

		xoffset *= cam->sensivity;
		yoffset *= cam->sensivity;

		cam->yaw += xoffset;
		cam->pitch += yoffset;

		if (cam->pitch < (-89.9f ) - 90.0)
			cam->pitch = (-89.9f) - 90.0;
		else if (cam->pitch > 89.9f - 90.0)
			cam->pitch = 89.9f - 90.0;

		moving = true;
	}

	updateFPSView(cam);

	return moving;
}

void nerv::sendCameraInfo(camera* cam)
{
		glUniformMatrix4fv(15, 1, GL_FALSE, &cam->view[0][0]);
		glUniform3fv(20, 1, &cam->position[0]);
		glUniform3fv(25, 1, &cam->front[0]);
		glUniform3fv(26, 1, &cam->up[0]);
		glUniform3fv(27, 1, &cam->right[0]);
		glUniform1fv(28, 1, &cam->fov);
		glUniform1fv(29, 1, &cam->aperture);
		glUniform1fv(30, 1, &cam->focusDistance);
}
