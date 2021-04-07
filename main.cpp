#include <iostream>
#include <glad/glad.h>
#include "src/config.h"

int main()
{
	std::cout << "loading config\n";
	nerv::config conf = nerv::loadConfig("E:\\nerv_engine\\nerv_pathtracer\\config.ini");

	int key = nerv::findConfigSubsector(&conf, "window");
	std::vector<int> winConfig = nerv::readConfigSubsector(&conf, key);

	//should output 768
	std::cout << winConfig[1] << std::endl;
}