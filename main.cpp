#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"


int main()
{
	nerv::init::launch();

	std::vector<float> vertices = {
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f
	};

	std::vector<size_t> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	nerv::object obj(vertices, new nerv::material(new nerv::texture("resources/default.png")));

	nerv::scene * worldScene = new nerv::scene();
	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION);
	cam->transform->translate(glm::vec3(0., 0., 3.));

	while (nerv::window::get().isOpen()) {
		

		cam->sendInfo();

		for (int i = 0; i < 10; i++)
		{
			obj.transform->translate(cubePositions[i]);
			obj.transform->rotate(20.0 * i + glfwGetTime(), glm::vec3(1., 0.0, 1.));
			obj.show();
		}

		nerv::keyboard::updateCameraKeyboard(cam);
		nerv::window::get().update();
	}

	delete worldScene;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}