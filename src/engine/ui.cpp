#pragma once

#include "ui.h"
#include "../in_out/windows/windows.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <glad/glad.h>

void nerv::ui::init()
{
	logger.warning("DEAR IMGUI", "Setting up dear imgui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	logger.warning("DEAR IMGUI", "linking to opengl and glfw");
	ImGui_ImplGlfw_InitForOpenGL(WINDOW_GLFW_DISPLAY, true);
	ImGui_ImplOpenGL3_Init("#version 450 core");
}

void nerv::ui::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void nerv::ui::draw(nerv::camera * cam)
{
	nerv::ui::newFrame();
	ImGui::SetNextWindowBgAlpha(0.35f);
	if (ImGui::Begin("Config"))
	{
		bool pressed = false;
		ImGui::Text("Camera");
		ImGui::Separator();
		pressed += ImGui::SliderFloat("FOV", &(cam->fov), 0.0, 180.0);
		pressed += ImGui::SliderFloat("focus Distance", &(cam->focusDistance), 0.0001, 50.0);
		pressed += ImGui::SliderFloat("apperture", &(cam->aperture), 0.0, 1.0);
		ImGui::Text("Material");
		ImGui::Separator();
		static float metal = 0.0;
		static float transmission = 0.0;
		static float roughtness = 0.0;
		pressed += ImGui::SliderFloat("metal", &metal, 0.0, 1.0);
		pressed += ImGui::SliderFloat("transmission", &transmission, 0.0, 1.0);
		pressed += ImGui::SliderFloat("roughtness", &roughtness, 0.0, 1.0);
		glUniform1fv(34, 1, &metal);
		glUniform1fv(35, 1, &transmission);
		glUniform1fv(36, 1, &roughtness);

		if (pressed) cam->isMoving = true;
	}
	ImGui::End();

	if (ImGui::Begin("FPS counter"))
	{
		int fps = 1. / nerv::window::get().getDeltaTime();
		ImGui::Text((std::to_string(fps) + " fps").c_str());
		ImGui::Text((std::to_string(nerv::window::get().getDeltaTime()) + " time between frame").c_str());
		if (!cam->isMoving)
		{
			cam->renderTime += nerv::window::get().getDeltaTime();
			cam->ssp += 1;
		}
		if (cam->isMoving)
		{
			cam->renderTime = 0.0f;
			cam->ssp = 1;
		}
		ImGui::Text(("render time : " + std::to_string(cam->renderTime) + " s").c_str());
		ImGui::Text(("samples : " + std::to_string(cam->ssp)).c_str());

	}
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void nerv::ui::clean()
{
	logger.warning("DEAR IMGUI", "using a broom to clean everything");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
