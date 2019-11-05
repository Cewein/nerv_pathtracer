#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"


int main()
{
	nerv::init::launch();

	std::vector<float> vertices = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	std::vector<size_t> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	nerv::object obj(vertices, indices, new nerv::material(new nerv::texture("resources/default.png")));

	nerv::scene * worldScene = new nerv::scene();
	nerv::camera cam(nerv::camera::projectionType::PERSPECTIVE_PROJECTION);

	while (nerv::window::get().isOpen()) {
		
		obj.transform->rotate(glm::radians(90.0) + glfwGetTime() * 50.0, glm::vec3(1., 0.0, 1.));
		cam.transform->translate(glm::vec3(0., 0., -3.));



		obj.material->use();
		cam.sendInfo();

		obj.show();
		nerv::window::get().update();
	}

	delete worldScene;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}