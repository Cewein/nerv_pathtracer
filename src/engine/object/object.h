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


		size_t VBO;
		size_t VAO;
		size_t EBO;

		nerv::mesh mesh;

		bool isElements;

		int size;
		object();

		void createObjectShader(std::string fragPath = std::string(), std::string vertPath = std::string());

	public:

		nerv::shader objectShader;

		//constructor and destructor

		object(std::vector<float> vertices, std::string fragPath = std::string(), std::string vertPath = std::string());
		object(std::vector<float> vertices, std::vector<size_t>  indices, std::string fragPath = std::string(), std::string vertPath = std::string());
		~object();

		void show();

		int getSize();

		nerv::mesh getMesh();
		nerv::object setMesh(nerv::mesh mesh);

 	};
}