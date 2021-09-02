#pragma once
#include "data.h"
#include "camera.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>



namespace nerv
{
	void createUI(GLFWwindow * win);
	void displayUI(GLFWwindow* win, renderData* info, camera* cam);
	void closeUI();
}
