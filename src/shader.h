#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

#define GLM_FORCE_MESSAGES // Or defined when building (e.g. -DGLM_FORCE_SWIZZLE)
#include <glm/glm.hpp>

#include "data.h"

namespace nerv
{ 
	static inline std::vector<float> vertices = {
		 1.0f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	static inline std::vector<size_t> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	class shader {
	private:

		//stuff for rendering the quad
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		std::vector<std::string> filePath;

		//only for vert+frag shader
		void bindFullquad();
		void showQuad();

	public:
		//shader id
		size_t id;

		//choose between vert+frag or compute
		bool isCompute;

		//given a path and a shader type you can load a create a shader
		size_t createShader(std::string path, int type);

		size_t createComputeProgram(size_t computeShader);
		size_t createMainProgram(size_t vertShader, size_t fragShader);	

		//import include for a shader file
		bool importInclude(std::string& data);

		//ouput shader error when compiling
		void programInfo(size_t id);

		//juste MANY MANY inline function for adding stuff to the shader
		inline void setInt(std::string name, int value) { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
		inline void setFloat(std::string name, float value) { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
		inline void setVec2(std::string name, float x, float y) { glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); }
		inline void setVec3(std::string name, glm::vec3 value) { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }
		inline void setVec3(std::string name, float x, float y, float z) { glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); }
		inline void setVec4(std::string name, glm::vec4 value) { glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }
		inline void setVec4(std::string name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); }
		inline void setMat3(std::string name, glm::mat3 value) { glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]); }
		inline void setMat4(std::string name, glm::mat4 value) { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]); }

		void activateImage(texture* img, char* name, int textureNumber);

		//create a compute shader
		shader(std::string path);

		//create a normal pipeline shader
		shader(std::string frag, std::string vert);

		void use();
		void reload();
	};
}
