#pragma once

#include "primitive.h"
#include <vector>

namespace nerv
{
	enum NODE_TYPE {
		AABB_BOX,
		TRIANGLE,
	};

	struct bvhNode
	{
		NODE_TYPE type;
		int primitiveOffset;
		bvhNode * left;
		bvhNode * right;
		nerv::AABB box;
	};

	bvhNode * createNode(std::vector<nerv::triangle>& tris, size_t start, size_t end);

	bool box_compare(nerv::triangle * a, nerv::triangle * b, int axis);
	bool box_x_compare(nerv::triangle* a, nerv::triangle* b);
	bool box_y_compare(nerv::triangle* a, nerv::triangle* b);
	bool box_z_compare(nerv::triangle* a, nerv::triangle* b);

	nerv::AABB surrondingBox(nerv::AABB box0, nerv::AABB box1);
}