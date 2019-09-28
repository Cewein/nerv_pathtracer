#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <utility>

#include "../../dependencies.h"

namespace nerv
{
	namespace config
	{
		//structure for each variable in a config file
		typedef struct settings {
			std::string name;
			double value;
		} setting;

		//load a config file and return a vector of all the data field for the specified field
		std::vector<nerv::config::setting> getSetting(std::string filePath, std::string field);
		
	}
}