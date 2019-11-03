#include "transform.h"

nerv::transform::transform()
{
	this->trans = glm::mat4(1.0);
}

void nerv::transform::reset()
{
	this->trans = glm::mat4(1.0);
}

void nerv::transform::translate(glm::vec3 move)
{
	this->trans = glm::translate(this->trans, move);
}

void nerv::transform::rotate(double angle, glm::vec3 axes)
{
	this->trans = glm::rotate(this->trans, (float)glm::radians(angle), axes);
}

void nerv::transform::scale(glm::vec3 scale)
{
	this->trans = glm::scale(this->trans, scale);
}

glm::mat4x4 nerv::transform::getTransformMatrix()
{
	return this->trans;
}

