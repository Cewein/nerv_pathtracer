#pragma once

#include "../../dependencies.h"
#include "../engine/object/primitive.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>

namespace nerv {

	typedef struct BVHbounds
	{
		int index = 0;
		glm::vec3 LLC = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 URC = glm::vec3(std::numeric_limits<float>::lowest());
		glm::vec3 centroid = glm::vec3(0);

		static BVHbounds uni(BVHbounds a, BVHbounds b);
		static BVHbounds uni(BVHbounds a, glm::vec3 b);
		static nerv::BVHbounds triangleBoundingInfo(nerv::primitive::triangle tris, int index = 0);

		glm::vec3 offset(glm::vec3 point) const;
		float surfaceArea();

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