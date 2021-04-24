#include "shader.h"

size_t nerv::shader::createShader(std::string path, int type)
{
	return size_t();
}

bool nerv::shader::importInclude(std::string& data)
{
	return false;
}

nerv::shader::shader(std::string path)
{
	id = 0;
	type = 0;
}

nerv::shader::shader(std::string frag, std::string vert)
{
	id = 0;
	type = 0;
}
