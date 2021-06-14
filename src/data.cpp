#pragma once
#include "data.h"
#include "primitive.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tiny_obj_loader.h>

#include <logarsh.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdlib>
#include <ctime>

size_t nerv::createBuffer(int size, void* data, int id, int bufferType)
{
	unsigned int buffer = 0;
	glGenBuffers(1, &buffer); 
	glBindBuffer(bufferType, buffer);

	glBufferData(bufferType, size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(bufferType, id, buffer);
	glBindBuffer(bufferType, id);
	return buffer;
}

nerv::texture nerv::loadImage(const char* path)
{

	texture img;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path, &(img.width), &(img.height), &(img.nbChannel), 0);

	if (data)
	{
		glGenTextures(1, &(img.id));
		glBindTexture(GL_TEXTURE_2D, img.id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (img.nbChannel)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}
	
		stbi_image_free(data);
		logger.info("TEXUTRE", "succesfully loaded the texture located at " + std::string(path));
	}
	else
	{
		logger.info("TEXUTRE", "error while openning the texture located at " + std::string(path));
	}

	return img;
}

std::vector<nerv::triangle> nerv::loadObj(const char* objPath, const char * matPath)
{
	std::string inputfile = objPath;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = matPath; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::vector<nerv::triangle> triangles;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) 
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			nerv::triangle tri;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) 
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				if (v == 0)
					tri.v1 = glm::vec4(vx, vy, vz, 0.0);
				else if(v == 1)
					tri.v2 = glm::vec4(vx, vy, vz, 0.0);
				else
					tri.v3 = glm::vec4(vx, vy, vz, 0.0);

				tri.data = glm::vec4(0);

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];

			triangles.push_back(tri);
		}
	}

	return triangles;
}

nerv::material* nerv::genRandomMaterial(int numberOfMat)
{
	srand(static_cast <unsigned> (time(0)));

	material* matArr = new material[numberOfMat];
	for (int i = 0; i < numberOfMat-1;i++)
	{ 
		matArr[i].color.x = randomFloat();
		matArr[i].color.y = randomFloat();
		matArr[i].color.z = randomFloat();
		matArr[i].color.w = randomFloat(2.0, 3.0);
		matArr[i].metallic = 0.0;
		matArr[i].refractionIndex = randomFloat(1.2, 2.0);
		matArr[i].roughness = 0.0;
		matArr[i].transmission = 0.0;
	}

	matArr[numberOfMat - 1].color.x = randomFloat();
	matArr[numberOfMat - 1].color.y = randomFloat();
	matArr[numberOfMat - 1].color.z = randomFloat();
	matArr[numberOfMat - 1].color.w = 1.0;
	matArr[numberOfMat - 1].metallic = 0.0;
	matArr[numberOfMat - 1].refractionIndex = randomFloat(1.2, 2.0);
	matArr[numberOfMat - 1].roughness = 1.0;
	matArr[numberOfMat - 1].transmission = 0.0;

	return matArr;
}

nerv::sphere* nerv::genRandomSphere(int numberOfSphere, int numberOfMat)
{
	srand(static_cast <unsigned> (time(0)));
	sphere* sphereArr = new sphere[numberOfSphere];

	//display the sphere in a circle
	for (int i = 0; i < numberOfSphere-20; i++)
	{
		float angle = i * 3.14159265359 * 2.0 / (numberOfSphere-20.0);
		sphereArr[i].pos.x = glm::cos(angle) * 5;
		sphereArr[i].pos.y = 0.4;
		sphereArr[i].pos.z = glm::sin(angle) * 5;
		sphereArr[i].pos.w = 0.2;
		sphereArr[i].mat = glm::vec4(rand() / (RAND_MAX / numberOfMat));
	}

	for (int i = numberOfSphere - 20; i < numberOfSphere; i++)
	{
		float angle = i * 3.14159265359 * 2.0 / 20;
		sphereArr[i].pos.x = glm::cos(angle) * 3;
		sphereArr[i].pos.y = 0.0;
		sphereArr[i].pos.z = glm::sin(angle) * 3;
		sphereArr[i].pos.w = 0.4; 
		sphereArr[i].mat = glm::vec4(numberOfMat);
	}

	return sphereArr; 
}

void nerv::sendInfo(camera* cam, renderData* render, GLFWwindow* win)
{
	sendCameraInfo(cam);

	int width;
	int height; 

	glfwGetFramebufferSize(win, &width, &height);

	glUniform2i(31, width, height);
	glUniform1iv(33, 1, &(render->isMoving));

	glUniform1f(34, glfwGetTime()); 
}
