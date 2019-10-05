#pragma once

#include "../../dependencies.h"
#include "object/object.h"

#include <vector>
#include <variant>
#include <string>
#include <iostream>


namespace nerv {
	typedef std::variant<nerv::object *> nervTypes;

	typedef struct _node
	{
		std::string name;
		size_t id;
		nerv::nervTypes data;
	} node;

	class scene
	{
	public:

		std::vector<nerv::node> tree;
		
		void add(nervTypes data, std::string name = "object");

		void showTreeConsole();

		nerv::scene();

	};
}