#pragma once

#include "../../../dependencies.h"
#include <math.h>

namespace nerv
{
	namespace primitive
	{
		typedef struct triangles {
			float v1[4];
			float v2[4];
			float v3[4];
		}triangle;

		/*nerv::BVHBound triangleBoundingInfo(triangle tris, int index);*/
	};
};