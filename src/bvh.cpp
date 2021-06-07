#include "bvh.h"
#include "commun.h"
#include <algorithm>

nerv::AABB nerv::surrondingBox(nerv::AABB box0, nerv::AABB box1)
{
	nerv::AABB surrond;
	surrond.max = glm::max(box0.max, box1.max);
	surrond.min = glm::min(box0.min, box1.min);
	return  surrond;
}

bool nerv::box_compare(nerv::triangle * a, nerv::triangle * b, int axis)
{
	nerv::AABB box0;
	nerv::AABB box1;

	a->boundingBox(&box0);
	b->boundingBox(&box1);

	return box0.min[axis] < box1.min[axis];
}

bool nerv::box_x_compare(nerv::triangle* a, nerv::triangle* b) {
	return box_compare(a, b, 0);
}

bool nerv::box_y_compare(nerv::triangle* a, nerv::triangle* b) {
	return box_compare(a, b, 1);
}

bool nerv::box_z_compare(nerv::triangle* a, nerv::triangle* b) {
	return box_compare(a, b, 2);
}

nerv::bvhNode* nerv::createNode(std::vector<nerv::triangle>& tris, size_t start, size_t end)
{
	nerv::bvhNode* node;

	int axis = (int)randomFloat(2);

	auto comparator = (axis == 0) ? box_x_compare
					: (axis == 1) ? box_y_compare
								  : box_z_compare;

	size_t objectSpan = end - start;

	if(objectSpan == 1)
	{
		node->type = nerv::NODE_TYPE::TRIANGLE;
		node->primitiveOffset = start;
		node->left = node->right = nullptr;

		nerv::AABB box;
		tris[start].boundingBox(&box);
		node->box = box;
	}
	else
	{
		std::sort(tris.begin() + start, tris.begin() + end, comparator);

		node->type = nerv::NODE_TYPE::AABB_BOX;
		node->primitiveOffset = -1;

		int mid = start + objectSpan / 2;

		node->left = nerv::createNode(tris, start, mid);
		node->right = nerv::createNode(tris, mid, end);

		node->box = surrondingBox(node->left->box, node->right->box);
	}

	return node;
}