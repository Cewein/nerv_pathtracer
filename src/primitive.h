#pragma once
#define GLM_SWIZZLE
#include <glm/glm.hpp>

namespace nerv
{
	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
	};

	struct primitive
	{
		virtual bool boundingBox(AABB* out) { return true; };
	};

	struct triangle
	{
		glm::vec4 v1;
		glm::vec4 v2;
		glm::vec4 v3;
		glm::vec2 uv1;
		glm::vec2 uv2;
		glm::vec2 uv3;
		glm::vec2 data;

		bool boundingBox(AABB* out);
	};

	// for the pos vec4 the w componant is the raduis
	struct sphere 
	{
		glm::vec4 pos;
		glm::vec4 mat;

		bool boundingBox(AABB* out);

	};
}