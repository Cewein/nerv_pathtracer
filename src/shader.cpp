#include <fstream>
#include <iostream>
#include <logarsh.h>

#include "shader.h"
#include "commun.h"

//since this program is only for raytracing there is no need of a proper
//rather way to bind vertices so the is the only function that will be use to bind
//a full quad to the gpu that will serv as a canvas for rendering
void nerv::shader::bindFullquad()
{

	logger.info("QUAD BINDING", "size is : " + std::to_string(indices.size()));

	float vertices[] = {
		 1.0f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
		 1.0f, -1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
		-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
		-1.0f,  1.0f, 1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

}

//render the quad to display the result of the fragement shader
void nerv::shader::showQuad()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

size_t nerv::shader::createShader(std::string path, int type)
{
	//create a shader type
	logger.info("SHADER", "Creating shader type");
	size_t shdr = glCreateShader(type);

	//look for shader file and import header
	logger.info("SHADER", "reading shader file at " + path);
	std::string file = nerv::read(path);
	while (importInclude(file));

	const char* fileChar = file.c_str();
	
	//load and compile shader
	glShaderSource(shdr, 1, &fileChar, NULL);
	glCompileShader(shdr);

	int success;
	char infoLog[512];
	glGetShaderiv(shdr, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shdr, 512, NULL, infoLog);
		logger.error("SHADER", "Shader did not compile : \n" + std::string(infoLog));

	}
	else
	{
		logger.info("SHADER", "Shader loaded and compiled proprely");
	}

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

void nerv::shader::activateImage(texture* img, char* name, int textureNumber)
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, img->id);
	setInt(name, textureNumber);
}

nerv::shader::shader(std::string path)
{
	size_t computeShader = createShader(path, GL_COMPUTE_SHADER);

	this->id = glCreateProgram();

	glAttachShader(this->id, computeShader);
	glLinkProgram(this->id);

	programInfo(this->id);

	glDeleteShader(computeShader);

	isCompute = true;
}

nerv::shader::shader(std::string frag, std::string vert)
{
	logger.info("SHADER", "Creating frag and vertex shaders");
	size_t fragShader = createShader(frag, GL_FRAGMENT_SHADER);
	size_t vertShader = createShader(vert, GL_VERTEX_SHADER);

	logger.info("SHADER", "Creating shader program");
	this->id = glCreateProgram();

	glAttachShader(this->id, vertShader);
	glAttachShader(this->id, fragShader);
	glLinkProgram(this->id);

	programInfo(this->id);

	logger.info("SHADER", "Cleaning shader instance");
	glDeleteShader(fragShader);
	glDeleteShader(vertShader);

	isCompute = false;

	logger.info("SHADER", "Binding full quad");
	bindFullquad();
}

void nerv::shader::use()
{
	if (isCompute)
	{
		
	}
	else
	{
		glUseProgram(id);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
