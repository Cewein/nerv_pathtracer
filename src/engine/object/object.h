#include <vector>

#include "../../../dependencies.h"
#include "../shader/shader.h"

namespace nerv
{
	struct Mesh
	{
		std::vector<float> vertices;
		std::vector<size_t> indices;
	};

	class object
	{
	private:


		size_t VBO;
		size_t VAO;
		size_t EBO;

		nerv::Mesh mesh;

		bool isElements;

		int size;
		object();

		void createShader(std::string fragPath = std::string(), std::string vertPath = std::string());

	public:

		shader objectShader;

		//constructor and destructor

		object(std::vector<float> vertices, std::string fragPath = std::string(), std::string vertPath = std::string());
		object(std::vector<float> vertices, std::vector<size_t>  indices, std::string fragPath = std::string(), std::string vertPath = std::string());
		~object();

		void show();

		int getSize();

		nerv::Mesh getMesh();
		nerv::object setMesh(nerv::Mesh mesh);

 	};
}