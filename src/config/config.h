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
		typedef struct settings {
			std::string name;
			double value;
		} setting;

		std::vector<setting> getSetting(std::string filePath, std::string field)
		{
			logger.startInit("Config for " + field);
			std::vector<setting> data;
			logger.initLog("open config file");
			std::ifstream conf(filePath);
			if (conf.is_open())
			{
				logger.initLog("file is open");
				std::string line;
				bool inField = false;
				logger.initLog("start of the search");
				while (std::getline(conf,line))
				{
					if (line[0] == '[' && inField)
						break;
					if (inField)
					{
						std::cout << line << std::endl;
						std::size_t pos = line.find('=');
						setting stg = {
							line.substr(0, pos),
							atof(line.substr(pos + 1).c_str())
						};
						data.push_back(stg);
					}
					if (line[0] == '[' && line.find(field) != std::string::npos)
					{
						std::cout << "find " << field << " in " << line << " at " << line.find(field)<< std::endl;
						inField = !inField;
					}
				}
			}
			else
			{
				logger.initLog("can't open file");
			}
			logger.initLog("end of the search");
			logger.endInit();
			return data;
			
		}
	}
}