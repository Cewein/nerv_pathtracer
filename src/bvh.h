#pragma once

#include "primitive.h"

namespace nerv
{
	struct bvhNode
	{
		bvhNode left;
		bvhNode right;
		nerv::AABB box;
	};
}