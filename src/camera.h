#pragma once
#include "config.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


namespace nerv 
{
	struct camera
	{
		//position value
		glm::vec3 position;
		glm::mat4 view;
		glm::vec3 up;
		glm::vec3 front;
		glm::vec3 right;
		float yaw;
		float pitch;

		//controle value
		float speed;
		float sensivity;
		float lastX;
		float lastY;
		float LastFrameTime;

		//lens value
		float fov;
		float aperture;
		float focusDistance;
	};

	//create a camera
	camera createCamera(nerv::config * conf);

	//get the fps view from a camera
	//update the up, front and right vector
	void updateFPSView(camera * cam);

	//update the position and the lastX, lastY
	bool updateCamera(camera * cam ,GLFWwindow * win);

	//send the information of the ginven camera to a shade
	void sendCameraInfo(camera * cam);
}