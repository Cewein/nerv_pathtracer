#pragma once
#include <glad/glad.h>
#include "object.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../../Dependencies/tiny_obj_loader.h"

nerv::object::object(std::vector<float> &vertices, nerv::material * material)
{
	this->size = (vertices.size() / 8);
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

	setMaterial(material);
	this->transform = new nerv::transform();
}

nerv::object::object(std::vector<float> &vertices, std::vector<size_t> &indices, nerv::material * material)
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

	setMaterial(material);
	this->transform = new nerv::transform();
}

nerv::object::~object()
{
	delete(this->material);
	delete(this->transform);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void nerv::object::show()
{
	this->material->use();
	this->material->shaderprog->setMat4("transform", this->transform->getTransformMatrix());
	this->material->shaderprog->setVec2("iResolution", nerv::window::get().width, nerv::window::get().height);
	this->material->shaderprog->setFloat("iTime", glfwGetTime());
	glBindVertexArray(this->VAO);
	if (this->isElements)
		glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_INT, 0);
	else
	glDrawArrays(GL_TRIANGLES, 0, this->size);
}

std::vector<nerv::triangle> nerv::object::loadObj(std::string path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shape;
	std::vector<tinyobj::material_t> material;

	std::string warn;
	std::string err;

	logger.startInit("TINYOBJLOADER");
	bool ret = tinyobj::LoadObj(&attrib, &shape, &material, &warn, &err, path.c_str());

	if (!warn.empty()) {
		std::cout << std::endl << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << std::endl << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	size_t index_offset = 0;
	std::vector<triangle> triangles;
	int id = 1;
	for (size_t s = 0; s < shape.size(); s++) {
		for (size_t f = 0; f < shape[s].mesh.num_face_vertices.size(); f++) {
			int fv = shape[s].mesh.num_face_vertices[f];

			triangle t;

			tinyobj::index_t idx = shape[s].mesh.indices[index_offset];
			t.v1[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v1[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v1[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v1[3] = 1.0f;
			idx = shape[s].mesh.indices[index_offset + 1];
			t.v2[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v2[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v2[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v2[3] = 1.0f;
			idx = shape[s].mesh.indices[index_offset + 2];
			t.v3[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v3[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v3[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v3[3] = 1.0f;

			triangles.push_back(t);

			index_offset += fv;
		}
	}

	logger.initLog("nb vertices : " + std::to_string(attrib.vertices.size()));
	logger.initLog("nb indice : " + std::to_string(shape[0].mesh.indices.size()));
	logger.endInit();

	logger.info("SSBO DATA", "size of data is : " + std::to_string(sizeof(triangle) * triangles.size()));

	return triangles;
}

int nerv::object::getSize()
{
	return this->size;
}

void nerv::object::setMaterial(nerv::material * material)
{
	if (material == nullptr)
		this->material = new nerv::material(
			new nerv::texture("resources/default.png"),
			new nerv::shader(std::string(), std::string())
		);
	else
		this->material = material;
}
