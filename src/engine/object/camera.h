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
		
		//send info to the shader in use
		void sendInfo();
		inline void sendViewInfoToShader(bool value) { sendViewInfo = value; }
	};

}