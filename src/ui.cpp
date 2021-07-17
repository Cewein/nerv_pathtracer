#pragma once
#include "ui.h"

#include <cstdlib>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void nerv::createUI(GLFWwindow* win)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 450 core");
}

void nerv::displayUI(renderData* info, camera * cam)
{
    info->isMoving = false;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	if (ImGui::Begin("info"))
	{
		ImGui::Text("Camera info");
        ImGui::Separator();
        ImGui::Text(("Samples : " + std::to_string(info->spp)).c_str());
        info->spp += 1;
	}

    if (ImGui::Begin("config"))
    {
        ImGui::Text("Camera");
        ImGui::Separator();
        info->isMoving += ImGui::SliderFloat("FOV", &(cam->fov), 0.0001, 180);
        info->isMoving += ImGui::SliderFloat("Aperture", &(cam->aperture), 0.0001, 1.0);
        info->isMoving += ImGui::SliderFloat("focus Distance", &(cam->focusDistance), 0.0001, 20);
        ImGui::Separator();
        ImGui::Text("Rendering");
        ImGui::Separator();
        info->isMoving += ImGui::SliderInt("Number of bounces", &(info->maxBounce), 0, 10);
        ImGui::Separator();
        ImGui::Text("Scene");
        ImGui::Separator();

        info->isMoving += ImGui::Checkbox("dark room mode", &(info->darkmode));
        
    } 
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (info->isMoving)
        info->spp = 1;
}

void nerv::closeUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
