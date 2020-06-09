#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"

 typedef struct spheres {
	float pos[4];
	float rmfr[4]; //raduis material fuzz refractionIndex
	float color[4];
}sphere;

 void changeCamPitch(nerv::camera * cam, float newPitch)
 {
	 cam->pitch += newPitch;
	 glm::vec3 front;

	 front.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	 front.y = sin(glm::radians(cam->pitch));
	 front.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));

	 cam->front = glm::normalize(front);
 }

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

	

	nerv::object scene(vertices,indices, new nerv::material(new nerv::texture("resources/greenwich_park_02.jpg"), new nerv::shader("shader/raytraced.frag.glsl", "shader/static.vert.glsl")));
	nerv::object post(vertices, indices, new nerv::material(framebuffer->frameTexture, new nerv::shader("shader/postprocess.frag.glsl")));

	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION, 90.0f);
	cam->transform->translate(glm::vec3(0., 1., 3.));
	
	int nbSample = 1;
	

	//test code for obj loading

	std::vector<nerv::triangle> triangles = nerv::object::loadObj("model/rab3.obj");

	size_t ssbo = nerv::shader::createBuffer(sizeof(nerv::triangle) * triangles.size(), triangles.data());
	size_t colorBuffer = nerv::shader::createBuffer(sizeof(float) * 4 * nerv::window::get().width * nerv::window::get().height, nullptr, 1);

	int i = 0;
	
	int ssp = 1;
	float renderTime = 0.0f;

	while (nerv::window::get().isOpen()) {

		//RENDERING 

		nerv::render::mainpass();
		
		cam->sendInfo();
		scene.material->shaderprog->setInt("size", triangles.size());
		if(nerv::camera::raytraced) scene.show();

		//UPDATE I/O

		cam->isMoving = false;
		nerv::keyboard::updateCameraKeyboard(cam);
		nerv::mouse::updateCameraMouse(cam);

		//UI

		nerv::ui::newFrame();
		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("FPS counter"))
		{
			int fps = 1. / nerv::window::get().getDeltaTime();
			ImGui::Text((std::to_string(fps) + " fps").c_str());
			ImGui::Text((std::to_string(nerv::window::get().getDeltaTime()) + " time between frame").c_str());
			if (!cam->isMoving)
			{
				renderTime += nerv::window::get().getDeltaTime();
				ssp += 1;
			}
			if (cam->isMoving)
			{
				renderTime = 0.0f;
				ssp = 1;
			}
			ImGui::Text(("render time : " + std::to_string(renderTime) + " s").c_str());
			ImGui::Text(("samples : " + std::to_string(ssp)).c_str());

		}
		ImGui::End();


		if (ImGui::Begin("Config"))
		{
			bool pressed = false;
			ImGui::Text("Camera");
			ImGui::Separator();
			pressed += ImGui::SliderFloat("FOV", &(cam->fov), 0.0, 180.0);
			pressed += ImGui::SliderFloat("focus Distance", &(cam->focusDistance), 0.0001, 50.0);
			pressed += ImGui::SliderFloat("apperture", &(cam->aperture), 0.0, 1.0);
			ImGui::Text("rendering");
			ImGui::Separator();
			scene.material->shaderprog->setInt("nbSample", nbSample);
			bool pres = ImGui::Checkbox("raytracing", &nerv::camera::raytraced);
			if (pres && !nerv::camera::raytraced) changeCamPitch(cam, 90.0f);
			if (pres && nerv::camera::raytraced) changeCamPitch(cam, -90.0f);
			if (pressed) cam->isMoving = true;
			

		}
		ImGui::End();
		nerv::ui::draw();

		//SWAP BUFFER
		nerv::window::get().update();
	}

	nerv::ui::clean();
	delete cam;
	//delete framebuffer;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}

