#include "config.h"
#include <iostream>
#include <string>
#include <cstring>

nerv::config nerv::loadConfig(char* path)
{
	nerv::config conf;

	conf.file.open(path);

	std::string line;

	//read the file and when we find a line
	//looking like : [name of subsector]
	//and we store the position of the cursor
	//and the name of subsector
	while (std::getline(conf.file, line))
	{
		if (line.find('[') != std::string::npos)
		{
			line.pop_back();
			line.erase(line.begin());

			int pos = conf.file.tellg();

			subsector subsec = std::make_tuple(line, pos);

			conf.sectors.push_back(subsec);
		}
	}

	//reset the file cursor
	conf.file.clear();

	return conf;
}

int nerv::findConfigSubsector(config* conf, const char* subsectorName)
{
	for (int i = 0; i < conf->sectors.size(); i++)
	{
		//comparte the tuple name and if it the same 
		//return starting position of the subsector
		std::string name = std::get<0>(conf->sectors[i]);
		if (!strcmp(name.c_str(), subsectorName))
			return std::get<1>(conf->sectors[i]);
	}

	return -1;
}

std::vector<int> nerv::readConfigSubsector(config* conf, int subsectorStart)
{

	if (subsectorStart == -1)
		return {};

	//put the cursor at the position
	//where the subsector start
	conf->file.seekg(subsectorStart);

	std::vector<int> val;

	std::string line;
	while (std::getline(conf->file, line))
	{
		
		if (line.find('[') != std::string::npos)
			break;
		
		//we add one so we skip the "=" sign in the string
		size_t pos = line.find("=")+1;

		std::string strVal = line.substr(pos);
		int value = std::stoi(strVal);

		val.push_back(value);
		
	}

	//reset the file cursor
	conf->file.clear();

	return val;
}
