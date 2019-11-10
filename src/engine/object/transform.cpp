#include "transform.h"

nerv::transform::transform()
{
	this->trans = glm::mat4(1.0);
	this->positionVec = glm::vec3(0.);
	this->scaleVec = glm::vec3(1.);
	this->rotateVec = glm::vec3(0.);
}

void nerv::transform::reset()
{
	this->positionVec = glm::vec3(0.);
	this->scaleVec = glm::vec3(1.);
	this->rotateVec = glm::vec3(0.);
	this->trans = glm::mat4(1.0);
}

void nerv::transform::translate(glm::vec3 move)
{
	this->positionVec = move;

}

void nerv::transform::rotate(double angle, glm::vec3 axes)
{
	this->rotateVec += (float)glm::radians(angle) * axes;
}

void nerv::transform::scale(glm::vec3 scale)
{
	this->scaleVec *=  scale;
}

void nerv::transform::rotateX(double angle)
{
}

void nerv::transform::rotateY(double angle)
{
}

void nerv::transform::rotateZ(double angle)
{
}

glm::mat4x4 nerv::transform::getTransformMatrix()
{
	this->trans = glm::translate(this->trans, this->positionVec);
	this->trans = glm::rotate(this->trans, this->rotateVec.x, glm::vec3(1, 0, 0));
	this->trans = glm::rotate(this->trans, this->rotateVec.y, glm::vec3(0, 1, 0));
	this->trans = glm::rotate(this->trans, this->rotateVec.z, glm::vec3(0, 0, 1));
	this->trans = glm::scale(this->trans, this->scaleVec);
	return this->trans;
}

