#pragma once
#include <vector>

#include "../../../dependencies.h"
#include "../shader/shader.h"
#include "texture.h"

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

		nerv::texture * texture;

		void createObjectShader(std::string fragPath = std::string(), std::string vertPath = std::string());

	public:

		nerv::shader objectShader;

		//constructor and destructor

		object(std::vector<float> &vertices, std::string fragPath = std::string(), std::string vertPath = std::string());
		object(std::vector<float> &vertices, std::vector<size_t> &indices, std::string fragPath = std::string(), std::string vertPath = std::string());
		~object();

		void show();
		void addTexture(std::string path);

		int getSize();

 	};
}