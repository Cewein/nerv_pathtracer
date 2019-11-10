#pragma once

#include "../../../dependencies.h"

namespace nerv
{
	class transform
	{
	private:

		//info for the set transform matrix
		glm::vec3 positionVec;
		glm::vec3 scaleVec;
		glm::vec3 rotateVec;

		//mat4 who containe all the info of the object,
		//she is reset ever time show() is used
		glm::mat4x4 trans;
	public:

		transform();
		void reset();

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
}