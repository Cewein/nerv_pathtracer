#include <fstream>
#include <tuple>
#include <vector>
#include <string>

typedef std::tuple<std::string, std::streampos> subsector;

namespace nerv {

	struct config
	{
		// config file
		std::fstream file;

		// this is use for storing were each subconfig sector are 
		// (ex: windows sector, keybind sector... ect)
		// we use that for letting the file open and not having
		// to rescan the whole file each time. we just need
		// to move the cursor to the wanted location
		//
		// char *: sector name
		// int: cursor position
		std::vector<subsector> sectors;
		
	};

	//load a config file into a config struct
	config loadConfig(char* path);

	//allow with a config and a string to find a subsector and cursor location
	//in a config, if not find return null
	int findConfigSubsector(config* conf, const char* subsectorName);
	std::vector<int> readConfigSubsector(config* conf, int subsectorStart);
}