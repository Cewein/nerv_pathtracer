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



	public:

		nerv::material * material;
		void setMaterial(nerv::material * material);

		//constructor and destructor

		object(std::vector<float> &vertices, nerv::material * material = nullptr);
		object(std::vector<float> &vertices, std::vector<size_t> &indices, nerv::material * material = nullptr);
		~object();

		void show();

		int getSize();

 	};
}