#include "BVH.h"

void nerv::BVHnodes::initLeaf(int first, int n, BVHbound & b)
{
	firstPrimOffset = first;
	nPrimitives = n;
	bounds = b;
	children[0] = children[1] = nullptr;
}

void nerv::BVHnodes::initInterior(int axis, BVHnodes * a, BVHnodes * b)
{
	children[0] = a;
	children[1] = b;
	nerv::primitive::bound bound = nerv::primitive::bound::uni(a->bounds.bound, b->bounds.bound);
	bounds = nerv::BVHbounds::createBVHBounds(bound);
	splitAxis = axis;
	nPrimitives = 0;
}

glm::vec3 nerv::BVHbounds::offset(glm::vec3 point) const
{
	glm::vec3 o = point - bound.pMin;

	if (bound.pMax.x > bound.pMin.x) o.x /= bound.pMax.x - bound.pMin.x;
	if (bound.pMax.y > bound.pMin.y) o.y /= bound.pMax.y - bound.pMin.y;
	if (bound.pMax.z > bound.pMin.z) o.z /= bound.pMax.z - bound.pMin.z;

	return o;
}

nerv::BVHbounds nerv::BVHbounds::createBVHBounds(nerv::primitive::bound bnd, int index)
{
	nerv::BVHbounds info{
	index,
	bnd.pMin,
	bnd.pMax,
	glm::vec3(0.5f * bnd.pMin + 0.5f * bnd.pMax)
	};

	return info;
}

nerv::BVHAccel::~BVHAccel()
{
}

nerv::BVHAccel::BVHAccel(std::vector<nerv::primitive::triangle>& p, int maxPrimsInNode, splitMethod method) : maxPrimsInNode(std::min(255, maxPrimsInNode)), primitives(p), method(method)
{
	logger.startInit("BVH");
	if (primitives.size() == 0)
		return;

	std::vector<BVHbounds> primInfo(primitives.size());
	for (int i = 0; i < primitives.size(); i++)
	{
		primInfo[i] = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::triangleBoundingInfo(primitives[i]),i);
	}
	logger.initLog("calculated all the primitives bounds");

	int totalNodes = 0;
	std::vector<primitive::triangle> orderedPrims;

	BVHnode * root = recursiveBuild(primInfo, 0, primitives.size(), &totalNodes, orderedPrims);
	logger.initLog("BVH tree build");
	 
	primitives.swap(orderedPrims);

	nodes = std::vector<linearBVHNode>(countNode(root));
	int offset = 0;
	flattenBVH(root, &offset,0);
	logger.initLog("BVH Flattened");
	freeBVHfromMemory(root);
	logger.initLog("removed BVH tree from memory");
	logger.endInit();
	logger.info("BVH", "Final size : " + std::to_string(nodes.size()));
}

nerv::BVHnode * nerv::BVHAccel::recursiveBuild(std::vector<BVHbound>& primInfo, int start, int end, int * totalNodes, std::vector<nerv::primitive::triangle>& orderedPrims)
{

	BVHnode * node = new BVHnode();

	//compute all the bounding volume of children primitives bounds
	BVHbound bounds;
	for (int i = start; i < end; i++)
	{
		bounds = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::uni(bounds.bound, primInfo[i].bound));
	}
	int nPrimitives = end - start;

	if (nPrimitives == 1)
	{
		int firstPrimOffset = orderedPrims.size();
		for (int i = start; i < end; i++)
		{
			int primNum = primInfo[i].index;
			orderedPrims.push_back(primitives[primNum]);
		}

		node->initLeaf(firstPrimOffset, nPrimitives, bounds);
		return node;
	}
	else
	{
		BVHbound centroidBounds;
		for (int i = start; i < end; i++)
			centroidBounds = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::uni(centroidBounds.bound, primInfo[i].centroid));

		//get bounding volume diagonal and find the longest axis
		glm::vec diag = centroidBounds.bound.pMax - centroidBounds.bound.pMin;
		int dim = 2;
		if (diag.x > diag.y && diag.x > diag.z)
			dim = 0;
		else if (diag.y > diag.z)
			dim = 1;

		int mid = (start + end) / 2;

		//if volume is null create the node and exit
		if (centroidBounds.bound.pMax[dim] == centroidBounds.bound.pMin[dim])
		{
			int firstPrimOffset = orderedPrims.size();
			for (int i = start; i < end; i++)
			{
				int primNum = primInfo[i].index;
				orderedPrims.push_back(primitives[primNum]);
			}

			node->initLeaf(firstPrimOffset, nPrimitives, bounds);
			return node;
		}
		else
		{
			if (nPrimitives <= 2)
			{
				mid = (start + end) / 2;
				std::nth_element(&primInfo[start], &primInfo[mid], &primInfo[end - 1] + 1, [dim](const BVHbound &a, const BVHbound &b)
				{
					return a.centroid[dim] < b.centroid[dim];
				});
			}
			else
			{
				constexpr int nBucket = 12;
				struct bucketInfo
				{
					int count = 0;
					BVHbound bounds;
				};
				bucketInfo buckets[nBucket];

				for (int i = start; i < end; i++)
				{
					int b = nBucket * centroidBounds.offset(primInfo[i].centroid)[dim];
					if (b == nBucket) b = nBucket - 1;
					buckets[b].count++;
					buckets[b].bounds = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::uni(buckets[b].bounds.bound, primInfo[i].bound));
				}

				float cost[nBucket - 1];
				for (int i = 0; i < nBucket - 1; i++)
				{
					BVHbound b0, b1;
					int count0, count1;
					count0 = count1 = 0;
					for (int j = 0; j <= i; j++)
					{
						b0 = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::uni(b0.bound, buckets[j].bounds.bound));
						count0 += buckets[j].count;
					}
					for (int j = i + 1; j < nBucket; j++)
					{
						b1 = nerv::BVHbounds::createBVHBounds(nerv::primitive::bound::uni(b1.bound, buckets[j].bounds.bound));
						count1 += buckets[j].count;
					}

					cost[i] = 1 + (count0 * b0.bound.surfaceArea() + count1 * b1.bound.surfaceArea()) / bounds.bound.surfaceArea();
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
					BVHbound * pmid = std::partition(&primInfo[start], &primInfo[end - 1] + 1, [=](const nerv::BVHbound &pi)
					{
						int b = nBucket * centroidBounds.offset(pi.centroid)[dim];
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
						int primNum = primInfo[i].index;
						orderedPrims.push_back(primitives[primNum]);
					}

					node->initLeaf(firstPrimOffset, nPrimitives, bounds);
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
	linear->pMin = glm::vec4(node->bounds.bound.pMin, depth);
	linear->pMax = glm::vec4(node->bounds.bound.pMax, depth);
	int myOffset = (*offset)++;
	if (node->nPrimitives > 0)
	{
		linear->primitiveOffset = node->firstPrimOffset;
		linear->nPrimitives = node->nPrimitives;
	}
	else
	{
		linear->axis = node->splitAxis;
		linear->nPrimitives = 0;
		flattenBVH(node->children[0], offset, depth + 1);
		linear->secondChildOffset = flattenBVH(node->children[1], offset, depth + 1);
	}
	return myOffset;
}
