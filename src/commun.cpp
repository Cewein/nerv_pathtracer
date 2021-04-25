#include "commun.h"

#include <iostream>


std::string nerv::read(std::string path)
{
	//load file
	std::string filestr;
	std::fstream file;
	file.open(path);

	//if file open, read it and put it into filestr
	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			filestr.append(line+"\n");
		}
		file.close();
	}

	//return file as a string
	return filestr;
}
