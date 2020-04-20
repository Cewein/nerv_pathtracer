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

void nerv::ui::draw()
{
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
