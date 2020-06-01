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

nerv::Quaternion::Quaternion(glm::vec4 vec)
{
	this->vec = vec;
}

double nerv::Quaternion::norm()
{
	return this->vec.length();
}

nerv::Quaternion nerv::Quaternion::conjugate()
{
	return Quaternion(glm::vec4(-this->vec.x, -this->vec.y, -this->vec.z, this->vec.w ));
}

glm::mat4x4 nerv::Quaternion::toMat4()
{
	return glm::mat4x4(
		glm::vec4(this->vec.w, -this->vec.x, -this->vec.y, -this->vec.z),
		glm::vec4(this->vec.x, this->vec.w, -this->vec.z, this->vec.y),
		glm::vec4(this->vec.y, this->vec.z, this->vec.w, -this->vec.x),
		glm::vec4(this->vec.z, -this->vec.y, this->vec.x, this->vec.w)
	);
}

float nerv::Quaternion::dot(Quaternion q)
{
	return glm::dot(this->vec, q.vec);
}

nerv::Quaternion nerv::Quaternion::operator+(Quaternion q)
{
	return Quaternion(this->vec + q.vec);
}

nerv::Quaternion nerv::Quaternion::operator*(Quaternion q)
{
	return Quaternion(
		glm::vec4(
			this->w()*q.x() + this->x() * q.w() + this->y() * q.z() - this->z() * q.y(), //x
			this->w()*q.y() - this->x() * q.z() + this->y() * q.w() + this->z() * q.x(), //y
			this->w()*q.z() + this->x() * q.y() - this->y() * q.x() + this->z() * q.w(), //z
			this->w()*q.w() - this->x() * q.x() - this->y() * q.y() - this->z() * q.z()  //w
		)
	);
}

nerv::Quaternion nerv::Quaternion::rotate(float angle, glm::vec3 axes)
{
	return Quaternion(
		glm::vec4(
			glm::sin(angle/2)*axes,
			glm::cos(angle/2.0f)
		)
	);
}

glm::mat3 nerv::Quaternion::toRotMat()
{
	return glm::mat3(
		glm::vec3(1 - 2*y()*y() - 2*z()*z(), 2*x()*y() - 2*z()*w()			, 2*x()*z() + 2*y()*w()			),
		glm::vec3(2 * x()*y() + 2 * z()*w(), 1 - 2 * x()*x() - 2 * z()*z()	, 2 * y()*z() - 2 * x()*w()		),
		glm::vec3(2 * x()*z() - 2 * y()*w(), 2 * y()*z() + 2 * x()*w()		, 1 - 2 * x()*x() - 2 * y()*y() )
	);
}
