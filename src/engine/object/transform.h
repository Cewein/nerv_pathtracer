#pragma once

#include "../../../dependencies.h"

namespace nerv
{
	class transform
	{
	public:
		//info for the set transform matrix
		glm::vec3 positionVec;
		glm::vec3 scaleVec;
		glm::vec3 rotateVec;

		transform();

		//movement, rotation and scale
		void translate(glm::vec3 move);
		void rotate(double angle, glm::vec3 axes);
		void scale(glm::vec3 scale);

		//rotation on one axe
		void rotateX(double angle);
		void rotateY(double angle);
		void rotateZ(double angle);

		//get transform matrices
		glm::mat4x4 getTransformMatrix();
	};


	class Quaternion
	{
	public:

		glm::vec4 vec;

		glm::mat4x4 toMat4();
		glm::mat3 toRotMat();

		double norm();

		Quaternion(glm::vec4 vec);

		Quaternion conjugate();
		float dot(Quaternion q);
		Quaternion operator+(Quaternion q);
		Quaternion operator*(Quaternion q);
		Quaternion rotate(float angle, glm::vec3 axes);

		inline float x() { return vec.x; };
		inline float y() { return vec.y; };
		inline float z() { return vec.z; };
		inline float w() { return vec.w; };
	};
}