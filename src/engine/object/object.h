#include "../../../dependencies.h"
#include "../shader/shader.h"

namespace nerv
{
	class object
	{
	private:

		shader objectShader;

		size_t VBO;
		size_t VAO;
		size_t EBO;


		float * vertices;
		size_t * indices;

		int size;
		object();
	public:

		object(float * vertex, int size);
		object(float * vertex, size_t * indices, int size);

		void show();

		int getSize();
	};
}