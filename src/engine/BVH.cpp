#include "BVH.h"

void nerv::BVHnodes::initLeaf(int first, int n, primitive::aabb & b)
{
	primitiveOffset = first;
	nPrimitives = n;
	box = b;
	children[0] = children[1] = nullptr;
}

void nerv::BVHnodes::initInterior(int axis, BVHnodes * a, BVHnodes *b)
{
	children[0] = a;
	children[1] = b;
	box = nerv::primitive::aabb::uni(a->box, b->box);
	primitiveOffset = -1;
	splitAxis = axis;
	nPrimitives = 0;
}

nerv::BVHAccel::~BVHAccel()
{
}

nerv::BVHAccel::BVHAccel(std::vector<nerv::primitive::triangle>& p, int maxPrimsInNode, splitMethod method) : maxPrimsInNode(std::min(255, maxPrimsInNode)), primitives(p), method(method)
{
	logger.startInit("BVH");
	if (primitives.size() == 0)
		return;

	std::vector<BVHnode> primInfo(primitives.size());
	for (int i = 0; i < primitives.size(); i++)
	{
		primInfo[i].box = nerv::primitive::aabb::triangleBoundingInfo(primitives[i]);
		primInfo[i].primitiveOffset = i;
	}
	logger.initLog("calculated all the primitives bounds");

	int totalNodes = 0;
	std::vector<primitive::triangle> orderedPrims;

	BVHnode * root = recursiveBuild(primInfo, 0, primitives.size(), &totalNodes, orderedPrims);
	logger.initLog("BVH tree build");
	 
	primitives.swap(orderedPrims);

	p = primitives;

	nodes = std::vector<linearBVHNode>(countNode(root));
	int offset = 0;
	flattenBVH(root, &offset,0);
	logger.initLog("BVH Flattened");
	freeBVHfromMemory(root);
	logger.initLog("removed BVH tree from memory");
	logger.endInit();
	logger.info("BVH", "Final size : " + std::to_string(nodes.size()));
}

nerv::BVHnode * nerv::BVHAccel::recursiveBuild(std::vector<BVHnode>& primInfo, int start, int end, int * totalNodes, std::vector<nerv::primitive::triangle>& orderedPrims)
{

	BVHnode * node = new BVHnode();

	//compute all the bounding volume of children primitives bounds
	primitive::aabb box;
	for (int i = start; i < end; i++)
	{
		box = nerv::primitive::aabb::uni(box, primInfo[i].box);
	}
	int nPrimitives = end - start;

	if (nPrimitives == 1)
	{
		int firstPrimOffset = orderedPrims.size();
		for (int i = start; i < end; i++)
		{
			int primNum = primInfo[i].primitiveOffset;
			orderedPrims.push_back(primitives[primNum]);
		}

		node->initLeaf(firstPrimOffset, nPrimitives, box);
		return node;
	}
	else
	{
		primitive::aabb centroidBounds;
		for (int i = start; i < end; i++)
			centroidBounds = nerv::primitive::aabb::uni(centroidBounds, primInfo[i].box.centroid);

		//get bounding volume diagonal and find the longest axis
		glm::vec diag = centroidBounds.pMax - centroidBounds.pMin;
		int dim = 2;
		if (diag.x > diag.y && diag.x > diag.z)
			dim = 0;
		else if (diag.y > diag.z)
			dim = 1;

		int mid = (start + end) / 2;

		//if volume is null create the node and exit
		if (centroidBounds.pMax[dim] == centroidBounds.pMin[dim])
		{
			int firstPrimOffset = orderedPrims.size();
			for (int i = start; i < end; i++)
			{
				int primNum = primInfo[i].primitiveOffset;
				orderedPrims.push_back(primitives[primNum]);
			}

			node->initLeaf(firstPrimOffset, nPrimitives, box);
			return node;
		}
		else
		{
			if (nPrimitives <= 2)
			{
				mid = (start + end) / 2;
				std::nth_element(&primInfo[start], &primInfo[mid], &primInfo[end - 1] + 1, [dim](const BVHnode &a, const BVHnode &b)
				{
					return a.box.centroid[dim] < b.box.centroid[dim];
				});
			}
			else
			{
				constexpr int nBucket = 12;
				struct bucketInfo
				{
					int count = 0;
					primitive::aabb box;
				};
				bucketInfo buckets[nBucket];

				for (int i = start; i < end; i++)
				{
					int b = nBucket * centroidBounds.offset(primInfo[i].box.centroid)[dim];
					if (b == nBucket) b = nBucket - 1;
					buckets[b].count++;
					buckets[b].box = nerv::primitive::aabb::uni(buckets[b].box, primInfo[i].box);
				}

				float cost[nBucket - 1];
				for (int i = 0; i < nBucket - 1; i++)
				{
					primitive::aabb b0, b1;
					int count0, count1;
					count0 = count1 = 0;
					for (int j = 0; j <= i; j++)
					{
						b0 = nerv::primitive::aabb::uni(b0, buckets[j].box);
						count0 += buckets[j].count;
					}
					for (int j = i + 1; j < nBucket; j++)
					{
						b1 = nerv::primitive::aabb::uni(b1, buckets[j].box);
						count1 += buckets[j].count;
					}

					cost[i] = 1 + (count0 * b0.surfaceArea() + count1 * b1.surfaceArea()) / box.surfaceArea();
				}

				float minCost = cost[0];
				int minCostSplitBucket = 0;
				for (int i = 1; i < nBucket - 1; i++)
				{
					if (cost[i] < minCost)
					{
						minCost = cost[i];
						minCostSplitBucket = i;
					}
				}

				float leafCost = nPrimitives;
				if (nPrimitives > maxPrimsInNode || minCost < leafCost)
				{
					nerv::BVHnode * pmid = std::partition(&primInfo[start], &primInfo[end - 1] + 1, [=](const nerv::BVHnode &pi)
					{
						int b = nBucket * centroidBounds.offset(pi.box.centroid)[dim];
						if (b == nBucket) b = nBucket - 1;
						return b <= minCostSplitBucket;
					});
					mid = pmid - &primInfo[0];
				}
				else
				{
					int firstPrimOffset = orderedPrims.size();
					for (int i = start; i < end; i++)
					{
						int primNum = primInfo[i].primitiveOffset;
						orderedPrims.push_back(primitives[primNum]);
					}

					node->initLeaf(firstPrimOffset, nPrimitives, box);
					return node;
				}
			}
			node->initInterior(dim,
				recursiveBuild(primInfo, start, mid, totalNodes, orderedPrims),
				recursiveBuild(primInfo, mid, end, totalNodes, orderedPrims));
		}
	}

	return node;
}

void nerv::BVHAccel::freeBVHfromMemory(BVHnode * node)
{
	if (node == nullptr)
		return;

	freeBVHfromMemory(node->children[0]);
	freeBVHfromMemory(node->children[1]);

	delete node;
}

int nerv::BVHAccel::countNode(BVHnode * node)
{
	int c = 1;
	if (node == nullptr)
		return 0;

	c += countNode(node->children[0]);
	c += countNode(node->children[1]);

	return c;
}

int nerv::BVHAccel::flattenBVH(BVHnode * node, int * offset, int depth)
{
	linearBVHNode * linear = &nodes[*offset];
	linear->pMin = glm::vec4(node->box.pMin, -1);
	linear->pMax = glm::vec4(node->box.pMax, -1);
	int myOffset = (*offset)++;
	if (node->nPrimitives > 0)
	{
		linear->primitiveOffset = node->primitiveOffset;
		linear->nPrimitives = node->nPrimitives;
	}
	else
	{
		linear->axis = node->splitAxis;
		linear->nPrimitives = 0;
		int ost = flattenBVH(node->children[0], offset, depth + 1);
		linear->pMin.w = ost;
		linear->secondChildOffset = flattenBVH(node->children[1], offset, depth + 1);
		linear->pMax.w = linear->secondChildOffset;

	}
	return myOffset;
}
