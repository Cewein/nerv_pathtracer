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

	nerv::object scene(vertices,indices, new nerv::material(new nerv::texture("resources/cape_hill.jpg"), new nerv::shader("shader/raytraced.frag.glsl", "shader/static.vert.glsl")));
	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION, 90.0f);
	cam->transform->translate(glm::vec3(0., 1., 3.));

	//test code for obj loading
	std::vector<nerv::primitive::triangle> triangles = nerv::object::loadObj("model/rabbit.obj");
	

	nerv::BVHAccel accelStruct(triangles, 1, nerv::BVHAccel::splitMethod::SAH);
	logger.info("BVH", "size of the flatten BVH : " + std::to_string(accelStruct.nodes.size() * sizeof(nerv::linearBVHNode)));

	size_t ssbo = nerv::shader::createBuffer(sizeof(nerv::primitive::triangle) * triangles.size(), triangles.data());
	size_t colorBuffer = nerv::shader::createBuffer(sizeof(float) * 4 * nerv::window::get().width * nerv::window::get().height, nullptr, 1);
	size_t bvh = nerv::shader::createBuffer(sizeof(nerv::linearBVHNode) * accelStruct.nodes.size(), accelStruct.nodes.data(), 2);

	int depth = 0;
	bool toogleDebug = false;

	while (nerv::window::get().isOpen()) {

		//RENDERING 
		cam->sendInfo();
		scene.material->shaderprog->setInt("size", triangles.size());
		if(nerv::camera::raytraced) scene.show();

		//UPDATE I/O

		cam->isMoving = false;
		nerv::keyboard::updateCameraKeyboard(cam);
		nerv::mouse::updateCameraMouse(cam);

		//UI
		nerv::ui::draw(cam);
		if (ImGui::Begin("Optimisation"))
		{
			bool pressed = false;
			bool debug = ImGui::Checkbox("Use BVH",&toogleDebug);
			pressed += debug;

			scene.material->shaderprog->setInt("bvhRendering", int(toogleDebug));

			if (pressed) cam->isMoving = true;
		}
		ImGui::End();

		//SWAP BUFFER
		nerv::window::get().update();
	}

	nerv::ui::clean();
	delete cam;

	nerv::window::get().close();
	return EXIT_SUCCESS;
}

