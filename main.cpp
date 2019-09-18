#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"


int main()
{
	nerv::init::launch();

	nerv::shader basic("shader/basic.vert.glsl", "shader/basic.frag.glsl");

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	nerv::object obj(vertices, sizeof(vertices));

	while (nerv::window::get().isOpen()) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		basic.use();
		
		obj.show();

		nerv::window::get().update();
	}

	nerv::window::get().close();
	return EXIT_SUCCESS;
}