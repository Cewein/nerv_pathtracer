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
nerv::camera::camera(nerv::config* conf)
{
	//read config file
	int sector = nerv::findConfigSubsector(conf, "camera");
	std::vector<int> camConf = nerv::readConfigSubsector(conf, sector);

	//see window.cpp
	sector = nerv::findConfigSubsector(conf, "window");
	std::vector<int> winConf = nerv::readConfigSubsector(conf, sector);

	//position value
	this->position = glm::vec3(-17.7, 5.1, 0.2);
	this->view = glm::mat4(1.0f);
	this->yaw = -88.0;
	this->pitch = -15.0;

	//controle value
	this->speed = camConf[0] / 10.0;
	this->sensivity = camConf[1] / 100.0;
	this->lastX = winConf[0] / 2;
	this->lastY = winConf[1] / 2;
	this->LastFrameTime = 0.0;

	//lens value
	this->fov = camConf[2];
	this->aperture = camConf[3];
	this->focusDistance = camConf[4];
}

void nerv::camera::updateFPSView(camera* cam)
{
	glm::vec3 eye = cam->position;

	float cosPitch = cosf(glm::radians(cam->pitch));
	float sinPitch = sinf(glm::radians(cam->pitch));
	float cosYaw = cosf(-glm::radians(cam->yaw));
	float sinYaw = sinf(-glm::radians(cam->yaw));

	glm::vec3 right = { cosYaw, 0, -sinYaw };
	glm::vec3 up = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	glm::vec3 front = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	right = glm::normalize(right);
	up = glm::normalize(up);
	front = glm::normalize(front);

	cam->view = {
		glm::vec4(right.x,            right.y,            right.z,      0),
		glm::vec4(up.x,               up.y,               up.z,      0),
		glm::vec4(front.x,            front.y,            front.z,      0),
		glm::vec4(-glm::dot(right, eye), -glm::dot(up, eye), -glm::dot(front, eye), 1)
	};
}

bool nerv::camera::updateCamera(camera* cam, GLFWwindow* win)
{
	bool moving = false;	
	float deltaTime = glfwGetTime() - cam->LastFrameTime;
	
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam->position += deltaTime * cam->speed * glm::vec3(cam->view[2]);
		moving = true; 
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam->position -= deltaTime * cam->speed * glm::vec3(cam->view[2]);
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam->position += deltaTime  * cam->speed * glm::vec3(cam->view[1]);
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cam->position -= deltaTime * cam->speed * glm::vec3(cam->view[1]);
		moving = true;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam->position += deltaTime * cam->speed * glm::normalize(glm::cross(glm::vec3(cam->view[2]), glm::vec3(cam->view[1])));
		moving = true;
	}  
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)   
	{
		cam->position -= deltaTime * cam->speed * glm::normalize(glm::cross(glm::vec3(cam->view[2]), glm::vec3(cam->view[1])));
		moving = true;
	}

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if (xpos != cam->lastX || ypos != cam->lastY)
	{
		float xoffset = -xpos + cam->lastX;
		float yoffset = ypos - cam->lastY;

		cam->lastX = xpos;
		cam->lastY = ypos;

		xoffset *= cam->sensivity;
		yoffset *= cam->sensivity;

		cam->yaw += xoffset;
		cam->pitch += yoffset;

		cam->pitch = fminf(fmaxf(cam->pitch, -89.9f), 89.9f);

		moving = true;
	}

	updateFPSView(cam);

	return moving;
}

void nerv::camera::updateTime(camera* cam)
{
	cam->LastFrameTime = glfwGetTime();
}

void nerv::camera::sendCameraInfo(camera* cam)
{
		glUniformMatrix4fv(15, 1, GL_FALSE, &cam->view[0][0]);
		glUniform3fv(20, 1, &cam->position[0]);
		glUniform1fv(28, 1, &cam->fov);
		glUniform1fv(29, 1, &cam->aperture);
		glUniform1fv(30, 1, &cam->focusDistance);
}
