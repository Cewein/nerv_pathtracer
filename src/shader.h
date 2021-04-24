#pragma once
#include <glad/glad.h>
#include <string>

#define GLM_FORCE_MESSAGES // Or defined when building (e.g. -DGLM_FORCE_SWIZZLE)
#include <glm/glm.hpp>

namespace nerv
{ 
	class shader {

		//shader id
		int id;

		//shader type (see GL_xxx_SHADER)
		int type;

		size_t createShader(std::string path, int type);
		bool importInclude(std::string& data);

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

		//create a compute shader
		shader(std::string path);

		//create a normal pipeline shader
		shader(std::string frag, std::string vert);
	};
}
