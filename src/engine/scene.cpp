#include "scene.h"

void nerv::scene::add(nervTypes data, std::string name)
{
	this->tree.push_back(node{ name, tree.size(), data });
}

void nerv::scene::showTreeConsole()
{
	for (int x = 0; x < tree.size(); x++)
	{
		std::cout << tree.at(x).name << std::endl;
	}
}

nerv::scene::scene()
{
}
