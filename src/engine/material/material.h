#pragma once

#include <string>

#include "../../../dependencies.h"
#include "shader.h"
#include "texture.h"
namespace nerv
{
	class material
	{
	private:


	public:
		nerv::shader * shaderprog;
		nerv::texture * texture;
		nerv::texture * normal;
		nerv::texture * specular;

		material();
		material(nerv::texture * texturePtr, nerv::shader * shaderPtr = nullptr);
		~material();

		void setShader(nerv::shader * shaderPtr);
		void createShader(std::string vertPath, std::string fragPath);

		void setTexture(nerv::texture * texturePtr);
		void createTexture(std::string texturePath);

		void use();


	};
}