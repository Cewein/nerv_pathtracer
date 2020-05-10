#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"
#include <random>

 typedef struct spheres {
	float pos[4];
	float rmfr[4]; //raduis material fuzz refractionIndex
	float color[4];
}sphere;

 std::mt19937 rng;

int main()
{

	nerv::init::launch();
	rng.seed(1337);

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

	const int size = 6;
	sphere * s = makeSphereData(size);
	
	int nbSample = 10;
	
	unsigned int ssbo = nerv::shader::createBuffer(sizeof(sphere) * size, s);

	while (nerv::window::get().isOpen()) {

		nerv::render::mainpass();

		
		cam->sendInfo();
		scene.material->shaderprog->setInt("size", size);
		scene.show();

		nerv::ui::newFrame();

		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("FPS counter"))
		{
			int fps = 1. / nerv::window::get().getDeltaTime();
			ImGui::Text((std::to_string(fps) + " fps").c_str());
			ImGui::Text((std::to_string(nerv::window::get().getDeltaTime()) + " time between frame").c_str());

			static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
			ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));
		}
		ImGui::End();


		if (ImGui::Begin("Config"))
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			ImGui::SliderFloat("FOV", &(cam->fov), 0.0, 180.0);
			ImGui::SliderFloat("focus Distance", &(cam->focusDistance), 0.0001, 50.0);
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
	delete s;
	//delete framebuffer;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}

sphere * makeSphereData(int size)
{

	std::uniform_real_distribution<float_t> floatGen(0.0f, 1.0f);

	sphere * s = new sphere[size];

	for (int i = 0; i < size - 1; i++)
	{
		float x = floatGen(rng) * 80.0f - 40.0f;
		float y = floatGen(rng) * 80.0f - 40.0f;
		float z = floatGen(rng) * 80.0f - 40.0f;
		float sz = 0.5f;

		s[i] =
		{
			{x, 0.f, z, 1.0f},
			{sz, floorf(floatGen(rng) * 3.f), floatGen(rng), floatGen(rng) + 1.f},
			{floatGen(rng),floatGen(rng), floatGen(rng), 0.0f}
		};
	}

	s[size - 1] =
	{
		{0.0f, -2000.5f, 0.0f, 1.0f},
		{2000.0f, 0.f, 0.f, 0.f},
		{0.4f,0.4f, 0.4f, 0.0f}
	};

	return s;
}