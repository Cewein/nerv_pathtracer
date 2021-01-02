#pragma once

#include "../../dependencies.h"
#include "../engine/object/primitive.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

namespace nerv {

	//typedef struct BVHbounds
	//{
	//	int index = 0;
	//	

	//	glm::vec3 centroid = glm::vec3(0.5f * bound.pMin + 0.5f * bound.pMax);
	//	glm::vec3 offset(glm::vec3 point) const;
	//	static BVHbounds createBVHBounds(nerv::primitive::aabb bnd, int index = 0);

	//} BVHbound;

	typedef struct BVHnodes
	{
		primitive::aabb box;
		BVHnodes * children[2];
		int splitAxis, primitiveOffset, nPrimitives;

		void initLeaf(int first, int n, primitive::aabb & b);
		void initInterior(int axis, BVHnodes * a, BVHnodes * b);

	} BVHnode;

	typedef struct linearBVHNode {
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