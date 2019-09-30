#include <vector>
#include <map>
#include <string>

#include "../engine.h"
#include "../../dependencies.h"

namespace nerv
{
	class scene
	{
	public:
		static std::map<int, nerv::object> objects;
		static void addObject(nerv::object);
		static void removeObejct(int id);
		static void removeObejct(std::string name);

	};
}