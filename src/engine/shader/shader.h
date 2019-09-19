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

		shader();
		shader(std::string vertPath, std::string fragPath);
		void use();

	};
}