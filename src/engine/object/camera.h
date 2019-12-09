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

		//placement of the camera
		nerv::transform * transform;
		
		float speed;
		float sensitivity;

		float lastX;
		float lastY;

		float yaw;
		float pitch;


		enum projectionType
		{
			PERSPECTIVE_PROJECTION,
			ORTHOGRAPHIC_PROJECTION
		};

		camera();
		camera(enum nerv::camera::projectionType projectionType);
		~camera();

		//view matrix function
		glm::vec3 getFront();
		glm::vec3 getRight();
		glm::vec3 getUp();

		//view type
		glm::mat4 getFPSView();
		
		//send info to the shader in use
		void sendInfo();
		inline void sendViewInfoToShader(bool value) { sendViewInfo = value; }
	};

}