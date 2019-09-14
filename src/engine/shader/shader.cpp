#pragma once
#include <glad/glad.h>
#include "shader.h"

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
		logger.warning("SHADER", "Shader loaded and compiled proprely");
	}

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
		logger.warning("SHADER", "Program loaded and compiled proprely");
	}
}

nerv::shader::shader(std::string vertPath, std::string fragPath)
{
	//TODO remove the dup here, to mayn time the same thing

	logger.warning("SHADER", "Compiling Vertex shader");
	const char * vertexFile = file::read(vertPath);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexFile, NULL);
	glCompileShader(vertexShader);
	shaderInfo(vertexShader);

	logger.warning("SHADER", "Compiling Fragment shader");
	const char * fragmentFile = file::read(fragPath);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentFile, NULL);
	glCompileShader(fragmentShader);
	shaderInfo(fragmentShader);

	logger.warning("SHADER", "Program in creation");
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	programInfo(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void nerv::shader::use()
{
	glUseProgram(shaderProgram);
}
