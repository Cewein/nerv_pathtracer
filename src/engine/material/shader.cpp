#pragma once
#include <glad/glad.h>
#include "shader.h"

size_t nerv::shader::liveProgram = NULL;

void nerv::shader::shaderInfo(size_t shaderId)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		logger.error("SHADER", "Shader did not compile : \n" + std::string(infoLog));

	}
	else
	{
		logger.info("SHADER", "Shader loaded and compiled proprely");
	}

}

unsigned int nerv::shader::createBuffer(int size, void * data, size_t id)
{
	unsigned int ssbo = 0;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, id, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	return ssbo;
}

void nerv::shader::programInfo(size_t program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		logger.error("SHADER", "Program did not compile : \n" + std::string(infoLog));
	}
	else
	{
		logger.info("SHADER", "Program loaded and compiled proprely");
	}
}

size_t nerv::shader::createShader(int shaderType, std::string path)
{
	size_t shdr;

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		logger.warning("SHADER", "Compiling Vertex shader");
		shdr = glCreateShader(GL_VERTEX_SHADER);
		if (path.empty())
			path = "shader/basic.vert.glsl";
		break;
	case GL_FRAGMENT_SHADER:
		logger.warning("SHADER", "Compiling Fragment shader");
		shdr = glCreateShader(GL_FRAGMENT_SHADER);
		if (path.empty())
			path = "shader/basic.frag.glsl";
		break;
	case GL_COMPUTE_SHADER:
		shdr = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		break;
	}

	std::string file = file::read(path);

	logger.info("SHADER", "Importing include");
	while (importInclude(file));

	const char * charPointer = file.c_str();

	glShaderSource(shdr, 1, &charPointer, NULL);
	glCompileShader(shdr);
	this->shaderInfo(shdr);

	return shdr;
}

bool nerv::shader::importInclude(std::string &data)
{
	size_t start = data.find("#include");
	if (start != std::string::npos)
	{
		size_t end = data.find('\n', start);
		std::string include = data.substr(start, end - start);
		logger.info("INCLUDE", "Include found : " + include);
		data.erase(start, end - start);

		include.erase(0, include.find('"')+1);
		include.erase(include.find('"'), include.size()+1);
		data.insert(start, file::read(include));
		return true;
	}
	return false;
}

nerv::shader::shader()
{
}

nerv::shader::shader(std::string fragPath, std::string vertPath)
{
	size_t vertexShader = this->createShader(GL_VERTEX_SHADER, vertPath );
	size_t fragmentShader = this->createShader(GL_FRAGMENT_SHADER, fragPath);

	logger.warning("SHADER", "Program in creation");
	this->shaderProgram = glCreateProgram();

	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);

	programInfo(this->shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void nerv::shader::use()
{
	if(compare(this))
		glUseProgram(nerv::shader::liveProgram);

	this->setVec2("screenResolution", nerv::window::get().width, nerv::window::get().height);
	this->setFloat("iTime", glfwGetTime());
}

bool nerv::shader::compare(nerv::shader * shader)
{
	if (shader->shaderProgram != nerv::shader::liveProgram)
	{
		nerv::shader::liveProgram = shader->shaderProgram;
		return true;
	}
	return false;
}
