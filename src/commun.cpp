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


//https://stackoverflow.com/questions/686353/random-float-number-generation
float nerv::randomFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float nerv::randomFloat(float max)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}

float nerv::randomFloat(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}