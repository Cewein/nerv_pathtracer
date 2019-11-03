#pragma once

#include "../../../dependencies.h"

namespace nerv
{
	class transform
	{
	private:

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

		//get transform matrices
		glm::mat4x4 getTransformMatrix();
	};
}