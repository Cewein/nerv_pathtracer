#include "config.h"

std::vector<nerv::config::setting> nerv::config::getSetting(std::string filePath, std::string field)
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

		while (std::getline(conf, line))
		{
			if (line[0] == '[' && inField)
				break;
			if (inField)
			{
				std::size_t pos = line.find('=');
				setting stg = {
					line.substr(0, pos),

					0
				};

				//you can had variable type here
				switch (line[pos + 1])
				{
				case '\'':
					stg.value = line[pos + 2];
					stg.value = (stg.value > 96 && stg.value < 123) ? stg.value - 32 : stg.value;
					break;
				case '"':
					stg.value = (line[pos + 2] == 't') ? 1 : 0;
					break;
				default:
					stg.value = atof(line.substr(pos + 1).c_str());
					break;
				}
				data.push_back(stg);
			}
			if (line[0] == '[' && line.find(field) != std::string::npos)
			{
				inField = !inField;
			}
		}
	}
	else
	{
		logger.initLog("can't open file");
		logger.crash("CONFIG", "Error no file found");
		exit(EXIT_FAILURE);
	}
	logger.initLog("end of the search");
	logger.endInit();
	return data;
}
