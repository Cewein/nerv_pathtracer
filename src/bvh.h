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

	struct linearBvhNode {
		glm::vec4 pMin = glm::vec4();
		glm::vec4 pMax = glm::vec4();
		int primitiveOffset = 0;
		int secondChildOffset = 0;
		int nPrimitives = 0;
		int axis = 0;
	};

	bvhNode * createNode(std::vector<nerv::triangle>& tris, size_t start, size_t end);

	void deleteBVHTree(bvhNode* bvh);
	int countNode(bvhNode* node);
	int flattenBVH(std::vector<linearBvhNode>& nodes, bvhNode* node, int* offset, int depth);

	bool box_compare(nerv::triangle  a, nerv::triangle  b, int axis);
	bool box_x_compare(nerv::triangle a, nerv::triangle b);
	bool box_y_compare(nerv::triangle a, nerv::triangle b);
	bool box_z_compare(nerv::triangle a, nerv::triangle b);

	nerv::AABB surrondingBox(nerv::AABB box0, nerv::AABB box1);
}