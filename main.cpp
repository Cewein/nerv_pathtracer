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

	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION, 90.0f);

	cam->transform->translate(glm::vec3(0., 0., 3.));

	int nbSample = 20;

	while (nerv::window::get().isOpen()) {

		nerv::render::mainpass();

		
		cam->sendInfo();
		scene.show();

		nerv::ui::newFrame();

		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("FPS counter"))
		{
			int fps = 1. / nerv::window::get().getDeltaTime();
			ImGui::Text((std::to_string(fps) + " fps").c_str());

			/*static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
			ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));*/
		}
		ImGui::End();


		if (ImGui::Begin("Config"))
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			ImGui::SliderFloat("FOV", &(cam->fov), 0.0, 180.0);
			ImGui::SliderFloat("focus Distance", &(cam->focusDistance), 0.0001, 20.0);
			ImGui::SliderFloat("apperture", &(cam->aperture), 0.0, 1.0);

			ImGui::Text("rendering");
			ImGui::Separator();
			ImGui::SliderInt("Number of sample", &nbSample, 1,150);
			scene.material->shaderprog->setInt("nbSample", nbSample);

		}
		ImGui::End();

		nerv::ui::draw();
		nerv::window::get().update();

		cam->isMoving = false;
		nerv::keyboard::updateCameraKeyboard(cam);
		nerv::mouse::updateCameraMouse(cam);
	}

	nerv::ui::clean();
	delete cam;
	//delete framebuffer;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}