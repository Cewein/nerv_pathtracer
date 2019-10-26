#pragma once

#include <string>

#include "../../../dependencies.h"
#include "shader.h"
#include "texture.h"
namespace nerv
{
	//Material class is use for set a "look" to a object
	class material
	{
	private:


	public:
		//Shader pointer, look for shader.h for more info
		nerv::shader * shaderprog;
		
		//texture poitner, look for texture.h for more info
		nerv::texture * texture;
		nerv::texture * normal;
		nerv::texture * specular;

		//default material is use to create a empty material
		material();

		//create a material with the nerv::texture and the nerv::shader send to 
		//the function, if no shaderPtr is given then it will take the basic frag and vert
		//located in the directory "shader"
		material(nerv::texture * texturePtr, nerv::shader * shaderPtr = nullptr);

		//clean all the pointer
		~material();

		//allow to set a new Shader pointer, look for shader.h for more info
		void setShader(nerv::shader * shaderPtr);

		//allow to set a new texture poitner, look for texture.h for more info
		void setTexture(nerv::texture * texturePtr);

		//let the material be used on the render.
		void use();


	};
}