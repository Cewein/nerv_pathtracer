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

bool nerv::box_compare(nerv::triangle a, nerv::triangle b, int axis)
{
	nerv::AABB box0;
	nerv::AABB box1;

	a.boundingBox(&box0);
	b.boundingBox(&box1);

	return box0.min[axis] < box1.min[axis];
}

bool nerv::box_x_compare(nerv::triangle a, nerv::triangle b) {
	return box_compare(a, b, 0);
}

bool nerv::box_y_compare(nerv::triangle a, nerv::triangle b) {
	return box_compare(a, b, 1);
}

bool nerv::box_z_compare(nerv::triangle a, nerv::triangle b) {
	return box_compare(a, b, 2);
}

nerv::bvhNode* nerv::createNode(std::vector<nerv::triangle>& tris, size_t start, size_t end)
{
	nerv::bvhNode* node = new nerv::bvhNode();

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

void nerv::deleteBVHTree(bvhNode* bvh)
{
	if (bvh == nullptr)
		return;

	deleteBVHTree(bvh->left);
	deleteBVHTree(bvh->right);

	delete bvh;
}

int nerv::countNode(bvhNode* node)
{
	int c = 1;
	if (node == nullptr)
		return 0;

	c += countNode(node->left);
	c += countNode(node->right);

	return c;
}

int nerv::flattenBVH(std::vector<linearBvhNode> &nodes, bvhNode* node, int* offset, int depth)
{
	linearBvhNode linear = nodes[*offset];
	linear.pMin = glm::vec4(node->box.min, -1);
	linear.pMax = glm::vec4(node->box.max, -1);
	int myOffset = (*offset)++;
	if (node->type != nerv::NODE_TYPE::AABB_BOX)
	{
		linear.primitiveOffset = node->primitiveOffset;
		linear.nPrimitives = 1;
	}
	else
	{
		linear.axis = 0;
		linear.nPrimitives = 0;
		int ost = flattenBVH(nodes,node->left, offset, depth + 1);
		linear.pMin.w = ost;
		linear.secondChildOffset = flattenBVH(nodes,node->right, offset, depth + 1);
		linear.pMax.w = linear.secondChildOffset;

	}
	return myOffset;
}