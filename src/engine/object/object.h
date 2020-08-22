 #pragma once
#include <vector>

#include "../../../dependencies.h"
#include "../material/material.h"
#include "primitive.h"
#include "transform.h"

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

	public:

		nerv::transform * transform;
		nerv::material * material;
		void setMaterial(nerv::material * material);

		//constructor and destructor

		object(std::vector<float> &vertices, nerv::material * material = nullptr);
		object(std::vector<float> &vertices, std::vector<size_t> &indices, nerv::material * material = nullptr);
		~object();

		void show();

		static std::vector<nerv::primitive::triangle> loadObj(std::string path);


		int getSize();

 	};
}