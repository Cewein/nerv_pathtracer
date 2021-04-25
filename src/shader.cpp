#include <fstream>
#include <iostream>

#include "shader.h"
#include "commun.h"

size_t nerv::shader::createShader(std::string path, int type)
{
	//create a shader type
	size_t shdr = glCreateShader(type);

	//look for shader file and import header
	std::string file = nerv::read(path);
	while (importInclude(file));

	const char* fileChar = file.c_str();
	
	//load and compile shader
	glShaderSource(shdr, 1, &fileChar, NULL);
	glCompileShader(shdr);
	return shdr;
}

bool nerv::shader::importInclude(std::string& data)
{
	//look for include into the file
	size_t start = data.find("#include");

	//if there is some open the file attached to it then
	//replace the include header with the file contente
	if (start != std::string::npos)
	{
		size_t end = data.find('\n', start);
		std::string include = data.substr(start, end - start);
		data.erase(start, end - start);

		include.erase(0, include.find('"') + 1);
		include.erase(include.find('"'), include.size() + 1);
		data.insert(start, nerv::read(include));
		return true;
	}
	return false;
}

void nerv::shader::programInfo(size_t id)
{
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << std::string(infoLog) << std::endl;
	}

}

nerv::shader::shader(std::string path)
{
	size_t computeShader = createShader(path, GL_COMPUTE_SHADER);

	this->id = glCreateProgram();

	glAttachShader(this->id, computeShader);
	glLinkProgram(this->id);

	programInfo(this->id);

	glDeleteShader(computeShader);
}

nerv::shader::shader(std::string frag, std::string vert)
{
	size_t fragShader = createShader(frag, GL_FRAGMENT_SHADER);
	size_t vertShader = createShader(vert, GL_VERTEX_SHADER);

	this->id = glCreateProgram();

	glAttachShader(this->id, fragShader);
	glAttachShader(this->id, vertShader);
	glLinkProgram(this->id);

	programInfo(this->id);

	glDeleteShader(fragShader);
	glDeleteShader(vertShader);
}
