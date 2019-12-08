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

		//matrix transform function
		glm::vec3 getFront();
		glm::vec3 getRight();
		glm::vec3 getUp();

		//get transform matrices
		glm::mat4x4 getTransformMatrix();
	};
}