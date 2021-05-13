#include <iostream>
#include <glad/glad.h>

#include "src/window.h"
#include "src/camera.h"
#include "src/shader.h"
#include "src/data.h"

int main()
{
	std::cout << "loading config\n";
	nerv::config conf = nerv::loadConfig("E:\\nerv_engine\\nerv_pathtracer\\config.ini");

	GLFWwindow* win = nerv::createWindow(&conf);
	nerv::camera cam = nerv::createCamera(&conf);

	nerv::renderData render = {
		0,
		5,
		false,
		0.0
	};

	nerv::shader mainShader("shader/raytraced.frag.glsl", "shader/static.vert.glsl");

	int width;
	int height;

	glfwGetFramebufferSize(win, &width, &height);

	size_t colorBuffer = nerv::createBuffer(sizeof(float) * 4 * width * height, nullptr, 1, GL_SHADER_STORAGE_BUFFER);
	//nerv::texture background = nerv::loadImage("resources/evening_road_01.jpg");

	//

	while (!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		nerv::sendCameraInfo(&cam);
		glUniform1iv(31, 1, &width);
		glUniform1iv(32, 1, &height);
		glUniform1iv(33, 1, &(render.isMoving));
		mainShader.setFloat("iTime", glfwGetTime());
		mainShader.setFloat("iDeltaTime", glfwGetTime());
		//mainShader.activateImage(&background, "text", 0);
		mainShader.setVec2("iResolution", width, height);
		mainShader.setInt("bvhRendering", 0);
		 
        mainShader.use();

		render.isMoving = nerv::updateCamera(&cam, win);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}