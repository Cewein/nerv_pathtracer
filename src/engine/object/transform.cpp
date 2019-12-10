#include "transform.h"

nerv::transform::transform()
{
	this->positionVec = glm::vec3(0.);
	this->scaleVec = glm::vec3(1.);
	this->rotateVec = glm::vec3(0.);
}

void nerv::transform::translate(glm::vec3 move)
{
	this->positionVec = move;

}

void nerv::transform::rotate(double angle, glm::vec3 axes)
{
	this->rotateVec = (float)glm::radians(angle) * axes;
}

void nerv::transform::scale(glm::vec3 scale)
{
	this->scaleVec =  scale;
}

void nerv::transform::rotateX(double angle)
{
	this->rotateVec.x = (float)glm::radians(angle);
}

void nerv::transform::rotateY(double angle)
{
	this->rotateVec.y = (float)glm::radians(angle);
}

void nerv::transform::rotateZ(double angle)
{
	this->rotateVec.z = (float)glm::radians(angle);
}

glm::mat4x4 nerv::transform::getTransformMatrix()
{
	glm::mat4 m = glm::translate(glm::mat4(1), this->positionVec);
	glm::quat qt = glm::quat(this->rotateVec);
	m *= glm::toMat4(qt);
	m = glm::scale(m, this->scaleVec);
	return m;
}

