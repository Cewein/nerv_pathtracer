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
	this->pitch = 0;
	this->sensitivity = 0.05f;
}

nerv::camera::camera(enum nerv::camera::projectionType projectionType)
{
	this->sendViewInfo = true;
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	logger.info("camera", "setting camera matrix");
	switch (projectionType)
	{
	case 0:
		logger.info("camera", "Perspective projection selected");
		this->projection = glm::perspective(glm::radians(45.0f), (float)window::get().width/ (float)window::get().height, 0.1f, 100.0f);
		break;
	case 1:
		logger.info("camera", "Orthogonal projection selected");
		this->projection = glm::ortho(0.0f, (float)window::get().width, 0.0f, (float)window::get().height, 0.1f, 100.0f);
		break;
	default:
		logger.info("camera", "Perspective projection selected");
		this->projection = glm::perspective(glm::radians(45.0f), (float)window::get().width / (float)window::get().height, 0.1f, 100.0f);
		break;
	}

	this->transform = new nerv::transform();
	this->speed = 5.f;

	this->lastX = nerv::window::get().width / 2;
	this->lastY = nerv::window::get().height / 2;
	this->yaw = 0;
	this->pitch = 0;
	this->sensitivity = 0.05f;
}

nerv::camera::~camera()
{
	delete(this->transform);
}

glm::vec3 nerv::camera::getFront()
{
	glm::mat4 m = this->view;
	return glm::vec3(m[2]);
}

glm::vec3 nerv::camera::getRight()
{
	glm::mat4 m = this->view;
	return glm::vec3(m[0]);
}

glm::vec3 nerv::camera::getUp()
{
	glm::mat4 m = this->view;
	return glm::vec3(m[1]);
}

glm::mat4 nerv::camera::getFPSView()
{
	glm::vec3 eye = this->transform->positionVec;

	float cosPitch = cosf(glm::radians(this->pitch));
	float sinPitch = sinf(glm::radians(this->pitch));
	float cosYaw = cosf(-glm::radians(this->yaw));
	float sinYaw = sinf(-glm::radians(this->yaw));

	glm::vec3 r = { cosYaw, 0, -sinYaw };
	glm::vec3 u = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	glm::vec3 f = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	r = glm::normalize(r);
	u = glm::normalize(u);
	f = glm::normalize(f);

	this->view = {
		glm::vec4(r.x,            u.x,            f.x,      0),
		glm::vec4(r.y,            u.y,            f.y,      0),
		glm::vec4(r.z,            u.z,            f.z,      0),
		glm::vec4(-glm::dot(r, eye), -glm::dot(u, eye), -glm::dot(f, eye), 1)
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
	}
	else
		glUniformMatrix4fv(15, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
}
