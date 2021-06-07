#pragma once

#include <fstream>
#include <string>

namespace nerv
{
	//read a file for a given file path
	std::string read(std::string path);

	//random number gen
	float randomFloat();
	float randomFloat(float max);
	float randomFloat(float min, float max);
}