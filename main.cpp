#include <glad/glad.h>
#include "dependencies.h"
#include "src/engine.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Dependencies/tiny_obj_loader.h"

 typedef struct spheres {
	float pos[4];
	float rmfr[4]; //raduis material fuzz refractionIndex
	float color[4];
}sphere;

 typedef struct triangles {
	 float v1[4];
	 float v2[4];
	 float v3[4];

	 void print()
	 {
		 logger.info("TRIANGLE", std::to_string(v1[0]) + " " + std::to_string(v1[1]) + " " + std::to_string(v1[2]));
		 logger.info("TRIANGLE", std::to_string(v2[0]) + " " + std::to_string(v2[1]) + " " + std::to_string(v2[2]));
		 logger.info("TRIANGLE", std::to_string(v3[0]) + " " + std::to_string(v3[1]) + " " + std::to_string(v3[2]));
	 }
 }triangle;

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

	nerv::object scene(vertices,indices, new nerv::material(nullptr, new nerv::shader("shader/raytraced.frag.glsl", "shader/static.vert.glsl")));
	nerv::object post(vertices, indices, new nerv::material(framebuffer->frameTexture, new nerv::shader("shader/postprocess.frag.glsl")));

	nerv::camera * cam = new nerv::camera(nerv::camera::projectionType::PERSPECTIVE_PROJECTION, 90.0f);
	cam->transform->translate(glm::vec3(0., 0., 3.));
	
	int nbSample = 1;
	

	//test code for obj loading

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shape;
	std::vector<tinyobj::material_t> material;

	std::string warn;
	std::string err;
	
	logger.startInit("TINYOBJLOADER");
	bool ret = tinyobj::LoadObj(&attrib, &shape, &material, &warn, &err, "model/rabbit.obj");

	if (!warn.empty()) {
		//std::cout << std::endl << warn << std::endl;
	}

	if (!err.empty()) {
		//std::cerr << std::endl << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}


	logger.initLog("nb vertices : " + std::to_string(attrib.vertices.size()));
	logger.initLog("nb indice : " + std::to_string(shape[0].mesh.indices.size()));
	logger.endInit();

	size_t index_offset = 0;
	std::vector<triangle> triangles;
	std::vector<float> obj;
	int id = 1;
	for (size_t s = 0; s < shape.size(); s++) {
		for (size_t f = 0; f < shape[s].mesh.num_face_vertices.size(); f++) {
			int fv = shape[s].mesh.num_face_vertices[f];

			triangle t;

			tinyobj::index_t idx = shape[s].mesh.indices[index_offset];
			t.v1[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v1[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v1[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v1[3] = 0;
			idx = shape[s].mesh.indices[index_offset + 1];
			t.v2[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v2[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v2[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v2[3] = 0;
			idx = shape[s].mesh.indices[index_offset + 2];
			t.v3[0] = attrib.vertices[3 * idx.vertex_index + 0];
			t.v3[1] = attrib.vertices[3 * idx.vertex_index + 1];
			t.v3[2] = attrib.vertices[3 * idx.vertex_index + 2];
			t.v3[3] = 0;

			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shape[s].mesh.indices[index_offset + v];
				obj.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				obj.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
				obj.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
				obj.push_back(attrib.normals[3 * idx.normal_index + 0]);
				obj.push_back(attrib.normals[3 * idx.normal_index + 1]);
				obj.push_back(attrib.normals[3 * idx.normal_index + 2]);
				obj.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
				obj.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
			}

			t.print();

			triangles.push_back(t);

			index_offset += fv;
		}
	}

	nerv::object object(obj);

	logger.info("SSBO DATA", "size of data is : " + std::to_string(sizeof(triangle)));

	unsigned int ssbo = nerv::shader::createBuffer(sizeof(triangle) * triangles.size(), triangles.data());

	float arr[] = 
	{ 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f 
	};
	int i = 0;
	

	while (nerv::window::get().isOpen()) {

		nerv::render::mainpass();

		
		cam->sendInfo();

		if (nerv::camera::raytraced)
		{
			scene.material->shaderprog->setInt("size", triangles.size());
			scene.show();
		}
		else
		{
			object.show();
		}

		nerv::ui::newFrame();

		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("FPS counter"))
		{
			int fps = 1. / nerv::window::get().getDeltaTime();
			ImGui::Text((std::to_string(fps) + " fps").c_str());
			ImGui::Text((std::to_string(nerv::window::get().getDeltaTime()) + " time between frame").c_str());

			arr[i] = nerv::window::get().getDeltaTime();

			i = (i + 1) % 96;

			ImGui::PlotLines("Frame Times", arr, 96);
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
			bool pressed = ImGui::Checkbox("raytracing", &nerv::camera::raytraced);
			if (pressed && !nerv::camera::raytraced) changeCamPitch(cam, 90.0f);
			if (pressed && nerv::camera::raytraced) changeCamPitch(cam, -90.0f);

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

