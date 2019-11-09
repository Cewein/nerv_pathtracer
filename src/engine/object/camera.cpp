#pragma once
#include <glad/glad.h>
#include "camera.h"

nerv::camera::camera()
{
	this->projection = glm::perspective(glm::radians(45.0f), (float)window::get().width / (float)window::get().height, 0.1f, 100.0f);
	this->transform = new nerv::transform();
}

nerv::camera::camera(enum nerv::camera::projectionType projectionType)
{
	this->projection = glm::mat4(1.0f);
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
		break;
	}

	this->transform = new nerv::transform();
}

nerv::camera::~camera()
{
	delete(this->transform);
}

void nerv::camera::sendInfo()
{
	glUniformMatrix4fv(glGetUniformLocation(nerv::shader::liveProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	
	//temp
	float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	nerv::transform * view = new nerv::transform();

	view->translate(glm::vec3(camX, 0.0, camZ));

	glUniformMatrix4fv(glGetUniformLocation(nerv::shader::liveProgram, "view"), 1, GL_FALSE, &glm::inverse(view->getTransformMatrix())[0][0]);
	this->transform->reset();
}
