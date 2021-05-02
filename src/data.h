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
	//create a SSBO buffer with a given size, a data pointer, and the id reference
	//we will have to use in the shader to call him

	size_t createBuffer(int size, void* data, int id, int bufferType);


	//texture stuff

	texture loadImage(const char* path);
	texture createImage(int width, int heigh, int nbChannel);

	void activateImage(texture* img, shader* prog, int textureNumber);

	
}