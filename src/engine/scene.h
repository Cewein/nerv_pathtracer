#pragma once

#include "../../dependencies.h"
#include "object/object.h"

#include <vector>
#include <variant>
#include <string>
#include <iostream>


namespace nerv {

	//type def for array node of a nervType, this is what allow scene graph to exist
	typedef std::variant<nerv::object *> nervTypes;

	//node struct for the scene graph
	typedef struct _node
	{
		std::string name;
		size_t id;
		nerv::nervTypes data;
	} node;

	class scene
	{
	public:

		//this is the actual scene graphs as of now nothing is saved
		std::vector<nerv::node> tree;
		
		void add(nervTypes data, std::string name = "object");

		void showTreeConsole();
		nerv::scene();

	};
}