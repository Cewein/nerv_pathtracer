#pragma once

#include "../../../dependencies.h"
#include "../material/shader.h"
#include "transform.h"

namespace nerv
{
	class camera
	{
	private:

		//projection matrix of the camera
		glm::mat4 projection;
		glm::mat4 view;
		bool sendViewInfo;

	public:
		//rendering and controle boolean (controle a mess up as of now)
		inline static bool raytraced = true;

		//placement of the camera
		nerv::transform * transform;
		
		float speed;
		float sensitivity;

		float lastX;
		float lastY;

		float yaw;
		float pitch;

		float fov;
		float aperture;
		float focusDistance;

		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		//render variable and rendering stat
		int ssp;
		float renderTime;
		bool isMoving;
		inline static bool focusing = true;

		enum projectionType
		{
			PERSPECTIVE_PROJECTION,
			ORTHOGRAPHIC_PROJECTION
		};

		camera();
		camera(enum nerv::camera::projectionType projectionType, float fov = 45.0);
		~camera();

		//view type
		glm::mat4 getFPSView();
		
		//send info to the shader in use
		void sendInfo();
		inline void sendViewInfoToShader(bool value) { sendViewInfo = value; }
	};
}