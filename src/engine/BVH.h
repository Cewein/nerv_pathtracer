#pragma once

#include "../../dependencies.h"
#include "../engine/object/primitive.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

namespace nerv {

	struct BVHnode
	{
		primitive::aabb box;
		BVHnode* children[2];
		int splitAxis, primitiveOffset, nPrimitives;

		void initLeaf(int first, int n, primitive::aabb & b);
		void initInterior(int axis, BVHnode* a, BVHnode* b);

	};

	struct linearBVHNode {
		glm::vec4 pMin = glm::vec4();
		glm::vec4 pMax = glm::vec4();
		int primitiveOffset = 0;
		int secondChildOffset = 0;
		int nPrimitives = 0 ;
		int axis = 0;
	};

	class BVHAccel
	{
	public:
		enum class splitMethod { SAH, HLBVH, middle, equalCounts };
		int maxPrimsInNode;
		splitMethod method;
		std::vector<nerv::primitive::triangle> primitives;
		std::vector<linearBVHNode> nodes;

		~BVHAccel();
		BVHAccel(std::vector<nerv::primitive::triangle> &p, int maxPrimsInNode, splitMethod method);
		BVHnode * recursiveBuild(std::vector<BVHnode> &primInfo, int start, int end, int *totalNodes, std::vector<nerv::primitive::triangle> &orderedPrims);
		static void freeBVHfromMemory(BVHnode * node);
		int countNode(BVHnode * node);
		int flattenBVH(BVHnode * node, int * offset, int depth);
	};
}