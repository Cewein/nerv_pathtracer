#include "material.h"

nerv::material::material()
{
	shaderprog = nullptr;
	texture = nullptr;
}

nerv::material::material(nerv::texture * texturePtr, nerv::shader * shaderPtr)
{
	this->normal = new nerv::texture("resources/default-normal.png");
	this->specular = new nerv::texture("resources/default-specular.png");
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
	delete(this->normal);
	delete(this->specular);
}

void nerv::material::setShader(nerv::shader * shaderPtr)
{
	this->shaderprog = shaderPtr;
}

void nerv::material::setTexture(nerv::texture * texturePtr)
{
	this->texture = texturePtr;
}

void nerv::material::use()
{
	if (this->shaderprog)
	{
		this->shaderprog->use();
	}
	if (this->texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture->getId());
		this->shaderprog->setInt("text", 0);
	}
	if (this->normal)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->normal->getId());
		this->shaderprog->setInt("norm", 1);
	}
	if (this->specular)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->specular->getId());
		this->shaderprog->setInt("spec", 2);
	}
}
