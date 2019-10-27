 #pragma once
#include <vector>

#include "../../../dependencies.h"
#include "../material/material.h"

namespace nerv
{
	class object
	{
	private:

		size_t VBO;
		size_t VAO;
		size_t EBO;

		bool isElements;

		int size;
		object();

		//mat4 who containe all the info of the object,
		//she is reset ever time show() is used
		glm::mat4x4 transform;
	public:

		nerv::material * material;
		void setMaterial(nerv::material * material);

		//constructor and destructor

		object(std::vector<float> &vertices, nerv::material * material = nullptr);
		object(std::vector<float> &vertices, std::vector<size_t> &indices, nerv::material * material = nullptr);
		~object();

		//movement, rotation and scale

		void translate(glm::vec3 move);
		void rotate(double angle, glm::vec3 axes);
		void scale(glm::vec3 scale);

		//get transform matrices
		glm::mat4x4 getTransformMatrix();

		void show();

		int getSize();

 	};
}