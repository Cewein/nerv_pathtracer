#pragma once

#include <glad/glad.h>

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

	//create a buffer with a given size, a data pointer, and the id reference
	//we will have to use in the shader to call him
	size_t createBuffer(int size, void* data, int id, int bufferType);

	//texture stuff
	texture loadImage(const char* path);

	//load .ob file
	int * loadObj(const char* objPath, const char* matPath = "./");
	
}