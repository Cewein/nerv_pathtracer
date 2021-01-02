#pragma once

#include "../../../dependencies.h"
#include <math.h>
#include <limits>
#include <algorithm>

namespace nerv
{
	namespace primitive
	{
		typedef struct triangles {
			float v1[4];
			float v2[4];
			float v3[4];
		}triangle;

		typedef struct aabbs
		{
			glm::vec3 pMin = glm::vec3(std::numeric_limits<float>::max());;
			glm::vec3 pMax = glm::vec3(std::numeric_limits<float>::lowest());

			static aabbs uni(aabbs a, aabbs b);
			static aabbs uni(aabbs a, glm::vec3 b);

			glm::vec3 centroid = glm::vec3(0.5f * (pMin + pMax));
			glm::vec3 offset(glm::vec3 point) const;

			static nerv::primitive::aabbs triangleBoundingInfo(nerv::primitive::triangle tris);
			float surfaceArea();

		} aabb;
	};
};