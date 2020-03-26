#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"


int main()
{
	nerv::init::launch();

	std::vector<float> vertices = {
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	std::vector<size_t> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	nerv::framebuffer * framebuffer = new nerv::framebuffer();

	nerv::object scene(vertices,indices, new nerv::material(nullptr, new nerv::shader("shader/raytraced.frag.glsl")));
	nerv::object post(vertices, indices, new nerv::material(framebuffer->frameTexture, new nerv::shader("shader/postprocess.frag.glsl")));

	nerv::scene * worldScene = new nerv::scene();
	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION);
	cam->transform->translate(glm::vec3(0., 0., 3.));

	while (nerv::window::get().isOpen()) {

		nerv::render::mainpass();
		
		cam->sendInfo();
		scene.show();

		/*if (cam->isMoving)
			logger.info("ENGINE", "Camera is moving");*/

		//nerv::render::postpass();
		//post.show();

		nerv::window::get().update();
		cam->isMoving = false;
		nerv::keyboard::updateCameraKeyboard(cam);
		nerv::mouse::updateCameraMouse(cam);
	}

	delete worldScene;
	delete cam;
	//delete framebuffer;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}