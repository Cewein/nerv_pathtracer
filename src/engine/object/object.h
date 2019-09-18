#include "../../../dependencies.h"

namespace nerv
{
	class object
	{
	private:
		size_t VBO;
		size_t VAO;

		float * vertex;
	public:

		object(float * vertex, int size);
		void show();
	};
}