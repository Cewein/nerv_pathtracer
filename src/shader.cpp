#include <fstream>

#include "shader.h"
#include "commun.h"

size_t nerv::shader::createShader(std::string path, int type)
{
	size_t shdr;

	switch (type)
	{
	case GL_VERTEX_SHADER:
		shdr = glCreateShader(GL_VERTEX_SHADER);
		break;
	case GL_FRAGMENT_SHADER:
		shdr = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GL_COMPUTE_SHADER:
		shdr = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		break;
	}

	return shdr;
}

bool nerv::shader::importInclude(std::string& data)
{
	size_t start = data.find("#include");
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

nerv::shader::shader(std::string path)
{
	id = 0;
}

nerv::shader::shader(std::string frag, std::string vert)
{
	id = 0;
}
