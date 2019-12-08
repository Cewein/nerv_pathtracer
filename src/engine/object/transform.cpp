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

glm::vec3 nerv::transform::getFront()
{
	glm::mat4 m = this->getTransformMatrix();
	return glm::vec3(m[2]);
}

glm::vec3 nerv::transform::getRight()
{
	glm::mat4 m = this->getTransformMatrix();
	return glm::vec3(m[0]);
}	
												
glm::vec3 nerv::transform::getUp()				
{												
	glm::mat4 m = this->getTransformMatrix();	
	return glm::vec3(m[1]);						
}

glm::mat4x4 nerv::transform::getTransformMatrix()
{
	glm::mat4 m = glm::translate(glm::mat4(1), this->positionVec);
	m = glm::rotate(m, this->rotateVec.x, glm::vec3(1, 0, 0));
	m = glm::rotate(m, this->rotateVec.y, glm::vec3(0, 1, 0));
	m = glm::rotate(m, this->rotateVec.z, glm::vec3(0, 0, 1));
	m = glm::scale(m, this->scaleVec);
	return m;
}

