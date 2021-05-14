#pragma once
#include <glad/glad.h>

#include "primitive.h"

namespace nerv
{
	struct renderData {
		int spp;
		int maxBounce;
		int isMoving;
		float renderTime;
	};

	struct texture {
		GLuint id;
		int width;
		int height;
		int nbChannel;
	};

	struct material {
		glm::vec4 color;
		float roughness;
		float metallic;
		float refractionIndex;
		float transmission;
	};

	//create a buffer with a given size, a data pointer, and the id reference
	//we will have to use in the shader to call him
	size_t createBuffer(int size, void* data, int id, int bufferType);

	//texture stuff
	texture loadImage(const char* path);

	//load .ob file
	int * loadObj(const char* objPath, const char* matPath = "./");

	//random number gen
	float randomFloat();
	float randomFloat(float max);
	float randomFloat(float min, float max);

	//scene generation
	material* genRandomMaterial(int numberOfMat);
	sphere * genRandomSphere(int numberOfSphere, int numberOfMat);
	
}