#include "data.h"
#pragma once

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
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &(img.width), &(img.height), &(img.nbChannel), 0);

	if (data)
	{
		glGenTextures(1, &(this->id));
		glBindTexture(GL_TEXTURE_2D, this->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (this->nbChannels)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}
	
		logger.initLog("uploaded texture to GC");
		stbi_image_free(data);
		logger.initLog("cleaned texture's data");
	}
	else
	{
		logger.initLog("error while openning the texture");
	}
	logger.endInit();
}
