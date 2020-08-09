#pragma once
#include <string>

#include "../../../dependencies.h"
#include "../../in_out/file.h"
#include "../../in_out/windows/windows.h"

namespace nerv 
{
	class shader
	{
	private:

		//id of the shaderProgram create in the openGL context
		size_t shaderProgram;

		//return debug info during the shader file reding process
		void shaderInfo(size_t shaderId);

		//return debug info durgin the shader binding process
		void programInfo(size_t program);

		//read a file into and out put a shader ID
		//shaderType is juste GL_XXX_SHADER macro
		size_t createShader(int shaderType, std::string path = std::string());
		bool importInclude(std::string &data);

	public:
		//actual live shader program
		static size_t liveProgram;

		//compare is wanted shader is not already use
		//is true is set the liveProgram variable to the wanted shader ID
		//is false (already in use) id do nothing
		static bool compare(nerv::shader * shader);
		static int unsigned createBuffer(int size, void* data, size_t id = 0);

		//juste MANY MANY inline function for adding stuff to the shader
		inline void setInt(std::string name, int value) { glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); }
		inline void setFloat(std::string name, float value) { glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); }
		inline void setVec2(std::string name, float x, float y) { glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y); }
		inline void setVec3(std::string name, glm::vec3 value) { glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); }
		inline void setVec3(std::string name, float x, float y, float z) { glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); }
		inline void setVec4(std::string name, glm::vec4 value) { glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); }
		inline void setVec4(std::string name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w); }
		inline void setMat3(std::string name, glm::mat3 value) { glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]); }
		inline void setMat4(std::string name, glm::mat4 value) { glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]); }

		shader();

		//constructor for the shader, if no vertPath is used it will use the default basic vert 
		//located in the directory "shader"
		shader(std::string fragPath, std::string vertPath = std::string());

		//tell openGL to use this shader program
		void use();

	};
}