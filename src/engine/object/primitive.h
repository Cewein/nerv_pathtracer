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

		typedef struct bounds
		{
			glm::vec3 LLC = glm::vec3(std::numeric_limits<float>::max());;
			glm::vec3 URC = glm::vec3(std::numeric_limits<float>::lowest());

			static bounds uni(bounds a, bounds b);
			static bounds uni(bounds a, glm::vec3 b);
			static nerv::primitive::bounds triangleBoundingInfo(nerv::primitive::triangle tris);
			float surfaceArea();

		} bound;
	};
};