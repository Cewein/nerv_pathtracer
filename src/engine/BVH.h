#pragma once

#include "../../dependencies.h"
#include "../engine/object/primitive.h"
#include <cmath>
#include <algorithm>
#include <vector>

namespace nerv {

	typedef struct BVHbounds
	{
		int index = 0;
		primitive::bound bound;

		glm::vec3 centroid = glm::vec3(0.5f * bound.LLC + 0.5f * bound.URC);
		glm::vec3 offset(glm::vec3 point) const;
		static BVHbounds creatyeBVHBounds(nerv::primitive::bound bnd, int index = 0);

	} BVHbound;

	typedef struct BVHnodes
	{
		BVHbound bounds;
		BVHnodes * children[2];
		int splitAxis, firstPrimOffset, nPrimitives;

		void initLeaf(int first, int n, BVHbound & b);
		void initInterior(int axis, BVHnodes * a, BVHnodes * b);

	} BVHnode;

	class BVHAccel
	{
	public:
		enum class splitMethod { SAH, HLBVH, middle, equalCounts };
		int maxPrimsInNode;
		splitMethod method;
		std::vector<nerv::primitive::triangle> primitives;
		BVHnode * root;

		BVHAccel(std::vector<nerv::primitive::triangle> &p, int maxPrimsInNode, splitMethod method);
		BVHnode * recursiveBuild(std::vector<BVHbound> &primInfo, int start, int end, int *totalNodes, std::vector<nerv::primitive::triangle> &orderedPrims);
		static void freeBVHfromMemory(BVHnode * node);
		int countNode(BVHnode * node);
	};
}