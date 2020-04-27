#pragma once
#include <glad/glad.h>
#include "camera.h"

nerv::camera::camera()
{
	this->view = glm::mat4(1.0f);
	this->projection = glm::perspective(glm::radians(45.0f), (float)window::get().width / (float)window::get().height, 0.1f, 100.0f);
	this->transform = new nerv::transform();
	this->speed = 0.5f;
	this->sendViewInfo = true;

	this->lastX = nerv::window::get().width / 2;
	this->lastY = nerv::window::get().height / 2;
	this->yaw = 0;
	this->pitch = -90.f;
	this->sensitivity = 0.05f;
	this->isMoving = true;
	this->fov = 90.0;
	this->focusDistance = 10.0;
	this->aperture = 0.0;
}

nerv::camera::camera(enum nerv::camera::projectionType projectionType, float fov)
{
	this->sendViewInfo = true;
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	logger.info("camera", "setting camera matrix");
	switch (projectionType)
	{
	case 0:
		logger.info("camera", "Perspective projection selected");
		this->projection = glm::perspective(glm::radians(fov), (float)window::get().width/ (float)window::get().height, 0.1f, 100.0f);
		break;
	case 1:
		logger.info("camera", "Orthogonal projection selected");
		this->projection = glm::ortho(0.0f, (float)window::get().width, 0.0f, (float)window::get().height, 0.1f, 100.0f);
		break;
	default:
		logger.info("camera", "Perspective projection selected");
		this->projection = glm::perspective(glm::radians(fov), (float)window::get().width / (float)window::get().height, 0.1f, 100.0f);
		break;
	}

	this->transform = new nerv::transform();
	this->speed = 5.f;
	this->fov = fov;
	this->focusDistance = 10.0;
	this->aperture = 0.0;

	this->lastX = nerv::window::get().width / 2;
	this->lastY = nerv::window::get().height / 2;
	this->yaw = 0;
	this->pitch = -90.f;
	this->sensitivity = 0.05f;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->right = glm::vec3(0.);
	this->isMoving = true;
}

nerv::camera::~camera()
{
	delete(this->transform);
}

glm::mat4 nerv::camera::getFPSView()
{
	glm::vec3 eye = this->transform->positionVec;

	float cosPitch = cosf(glm::radians(this->pitch));
	float sinPitch = sinf(glm::radians(this->pitch));
	float cosYaw = cosf(-glm::radians(this->yaw));
	float sinYaw = sinf(-glm::radians(this->yaw));

	right = { cosYaw, 0, -sinYaw };
	up = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	front = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	right = glm::normalize(right);
	up = glm::normalize(up);
	front = glm::normalize(front);

	this->view = {
		glm::vec4(right.x,            up.x,            front.x,      0),
		glm::vec4(right.y,            up.y,            front.y,      0),
		glm::vec4(right.z,            up.z,            front.z,      0),
		glm::vec4(-glm::dot(right, eye), -glm::dot(up, eye), -glm::dot(front, eye), 1)
	};

	return this->view;
}

//Allow to send info to a shader if the value "sendViewInfo is true
//This function do not use look at since a View matrix is just a
//Inverted tranform matrix.
void nerv::camera::sendInfo()
{
	glUniformMatrix4fv(10, 1, GL_FALSE, &projection[0][0]);
	if (sendViewInfo)
	{
		glUniformMatrix4fv(15, 1, GL_FALSE, &this->getFPSView()[0][0]);
		glUniformMatrix4fv(20, 1, GL_FALSE, &this->transform->getTransformMatrix()[0][0]);
		glUniform3fv(25, 1, &this->front[0]);
		glUniform3fv(26, 1, &this->up[0]);
		glUniform3fv(27, 1, &this->right[0]);
		glUniform1fv(28, 1, &this->fov);
		glUniform1fv(29, 1, &this->aperture);
		glUniform1fv(30, 1, &this->focusDistance);
	}
	else
		glUniformMatrix4fv(15, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
}
