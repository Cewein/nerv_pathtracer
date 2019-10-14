#pragma once
#include <glad/glad.h>
#include "object.h"

void nerv::object::createObjectShader(std::string fragPath, std::string vertPath)
{
	if (!fragPath.empty())
	{
		this->objectShader = nerv::shader("shader/basic.vert.glsl", fragPath);
	}
	else if (!fragPath.empty() && !vertPath.empty())
	{
		this->objectShader = nerv::shader(vertPath, fragPath);
	}
	else
	{
		this->objectShader = nerv::shader("shader/basic.vert.glsl", "shader/basic.frag.glsl");
	}
}

nerv::object::object(std::vector<float> &vertices, std::string fragPath, std::string vertPath)
{
	this->size = vertices.size();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->isElements = false;

	vertices.clear();

	this->createObjectShader(fragPath, vertPath);

}

nerv::object::object(std::vector<float> &vertices, std::vector<size_t> &indices, std::string fragPath, std::string vertPath)
{
	this->size = indices.size();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->isElements = true;

	vertices.clear();
	indices.clear();

	this->createObjectShader(fragPath, vertPath);
}

nerv::object::~object()
{
	delete(this->texture);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void nerv::object::show()
{
	this->objectShader.use();
	if (texture)
	{
		glBindTexture(GL_TEXTURE_2D, this->texture->getId());
	}
	glBindVertexArray(this->VAO);
	if (this->isElements)
		glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_INT, 0);
	else
	glDrawArrays(GL_TRIANGLES, 0, this->size);
}

void nerv::object::addTexture(std::string path)
{
	this->texture = new nerv::texture(path);
}

int nerv::object::getSize()
{
	return this->size;
}
