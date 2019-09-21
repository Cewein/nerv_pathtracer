#pragma once
#include <glad/glad.h>
#include "object.h"

nerv::object::object(std::vector<float> vertices)
{
	this->size = vertices.size();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	isElements = false;

	mesh.vertices = vertices;

	objectShader = shader("shader/basic.vert.glsl", "shader/basic.frag.glsl");

}

nerv::object::object(std::vector<float> vertices, std::vector<size_t> indices)
{
	this->size = indices.size();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	isElements = true;

	mesh.vertices = vertices;
	mesh.indices = indices;

	objectShader = shader("shader/basic.vert.glsl", "shader/basic.frag.glsl");
}

void nerv::object::show()
{
	objectShader.use();
	glBindVertexArray(VAO);
	if (isElements)
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	else
	glDrawArrays(GL_TRIANGLES, 0, size);
}

int nerv::object::getSize()
{
	return size;
}
