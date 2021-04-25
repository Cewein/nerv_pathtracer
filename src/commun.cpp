#include "commun.h"

#include <iostream>

std::string nerv::read(std::string path)
{
	std::string filestr;
	std::fstream file;
	file.open(path);
	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			filestr.append(line+"\n");
		}
		file.close();
	}

	return filestr;
}
