#include <vector>

#include "../../../dependencies.h"
#include "../shader/shader.h"

namespace nerv
{
	struct mesh
	{
		std::vector<float> vertices;
		std::vector<size_t> indices;
	};

	class object
	{
	private:

		shader objectShader;

		size_t VBO;
		size_t VAO;
		size_t EBO;

		mesh mesh;

		bool isElements;

		int size;
		object();
	public:

		object(std::vector<float> vertices);
		object(std::vector<float> vertices, std::vector<size_t>  indices);

		void show();

		int getSize();
	};
}