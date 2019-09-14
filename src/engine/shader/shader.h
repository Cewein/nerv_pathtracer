#pragma once
#include <string>

#include "../../../dependencies.h"
#include "../../in_out/file.h"

namespace nerv {
	class shader
	{
	private:
		
		size_t vertexShader;
		size_t fragmentShader;

		size_t shaderProgram;

		void shaderInfo(size_t shaderId);
		void programInfo(size_t program);

	public:
		shader(std::string vertPath, std::string fragPath);
		void use();
	};
}