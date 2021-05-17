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
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	if (ImGui::Begin("menu"))
	{
		ImGui::Text("Camera info");
        ImGui::Separator();
        ImGui::Text(("Samples : " + std::to_string(info->spp)).c_str());
        info->spp += 1;
        if(info->isMoving)
            info->spp = 1;
		ImGui::Separator();
		ImGui::Text("Material");
	}
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void nerv::closeUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
