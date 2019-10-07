#pragma once
#include <string>

#include "../../../dependencies.h"
#include "../../in_out/file.h"

namespace nerv {
	class shader
	{
	private:

		size_t shaderProgram;

		void shaderInfo(size_t shaderId);
		void programInfo(size_t program);

		size_t createShader(std::string path, int shaderType);

	public:
		static size_t liveProgram;
		static bool compare(nerv::shader * shader);

		inline void setInt(std::string name, int value) { glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); }
		inline void setFloat(std::string name, float value) { glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); }
		//inline void setVec3(char * name, vec3 value) { glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &value[0]); }
		inline void setVec3(std::string name, float x, float y, float z) { glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); }
		//inline void setVec4(char * name, vec4 value) { glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, &value[0]); }
		inline void setVec4(std::string name, float x, float y, float z, float w) { glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w); }
		//inline void setMat3(char * name, mat3 value) { glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &value[0][0]); }
		//inline void setMat4(char * name, mat4 value) { glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, &value[0][0]); }

		shader();
		shader(std::string vertPath, std::string fragPath);
		void use();

	};
}