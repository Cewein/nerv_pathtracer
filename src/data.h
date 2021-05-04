#pragma once
#include <glad/glad.h>

#include <stb_image.h>
#include <tiny_obj_loader.h>

#include "shader.h"

namespace nerv
{

	struct texture {
		size_t id;
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