#include "material.h"

nerv::material::material()
{
	shaderprog = nullptr;
	texture = nullptr;
}

nerv::material::material(nerv::texture * texturePtr, nerv::shader * shaderPtr)
{
	if (shaderPtr == nullptr)
		this->shaderprog = new nerv::shader(std::string(), std::string());
	else
		this->shaderprog = shaderPtr;
	this->texture = texturePtr;
}

nerv::material::~material()
{
	delete(this->shaderprog);
	delete(this->texture);
}

void nerv::material::setShader(nerv::shader * shaderPtr)
{
	this->shaderprog = shaderPtr;
}

void nerv::material::createShader(std::string vertPath, std::string fragPath)
{
	this->shaderprog = new nerv::shader(vertPath, fragPath);
}

void nerv::material::setTexture(nerv::texture * texturePtr)
{
	this->texture = texturePtr;
}

void nerv::material::createTexture(std::string texturePath)
{
	this->texture = new nerv::texture(texturePath);
}

void nerv::material::use()
{
	if (this->shaderprog)
	{
		this->shaderprog->use();
	}
	if (this->texture)
	{
		glBindTexture(GL_TEXTURE_2D, this->texture->getId());
	}
	
}
