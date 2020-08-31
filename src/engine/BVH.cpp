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
	bounds = nerv::BVHbound::uni(a->bounds, b->bounds);
	splitAxis = axis;
	nPrimitives = 0;
}

nerv::BVHbounds nerv::BVHbounds::uni(BVHbounds a, BVHbounds b)
{
	BVHbounds bound;
	bound.LLC = glm::vec3(std::min(a.LLC.x, b.LLC.x), std::min(a.LLC.y, b.LLC.y), std::min(a.LLC.z, b.LLC.z));
	bound.URC = glm::vec3(std::max(a.URC.x, b.URC.x), std::max(a.URC.y, b.URC.y), std::max(a.URC.z, b.URC.z));
	bound.centroid = glm::vec3(0.5f * bound.LLC + 0.5f * bound.URC);
	return bound;
}

nerv::BVHbounds nerv::BVHbounds::uni(BVHbounds a, glm::vec3 b)
{
	BVHbounds bound;
	bound.LLC = glm::vec3(std::min(a.LLC.x, b.x), std::min(a.LLC.y, b.y), std::min(a.LLC.z, b.z));
	bound.URC = glm::vec3(std::max(a.URC.x, b.x), std::max(a.URC.y, b.y), std::max(a.URC.z, b.z));
	bound.centroid = glm::vec3(0.5f * bound.LLC + 0.5f * bound.URC);
	return bound;
}

nerv::BVHbounds nerv::BVHbounds::triangleBoundingInfo(nerv::primitive::triangle tris, int index)
{
	float sx1 = tris.v1[0];
	float sx2 = tris.v2[1];
	float sx3 = tris.v3[2];

	float sy1 = tris.v1[0];
	float sy2 = tris.v2[1];
	float sy3 = tris.v3[2];

	float sz1 = tris.v1[0];
	float sz2 = tris.v2[1];
	float sz3 = tris.v3[2];

	float xmax = sx1 > sx2 ? (sx1 > sx3 ? sx1 : sx3) : (sx2 > sx3 ? sx2 : sx3);
	float ymax = sy1 > sy2 ? (sy1 > sy3 ? sy1 : sy3) : (sy2 > sy3 ? sy2 : sy3);
	float zmax = sz1 > sz2 ? (sz1 > sz3 ? sz1 : sz3) : (sz2 > sz3 ? sz2 : sz3);

	float xmin = sx1 < sx2 ? (sx1 < sx3 ? sx1 : sx3) : (sx2 < sx3 ? sx2 : sx3);
	float ymin = sy1 < sy2 ? (sy1 < sy3 ? sy1 : sy3) : (sy2 < sy3 ? sy2 : sy3);
	float zmin = sz1 < sz2 ? (sz1 < sz3 ? sz1 : sz3) : (sz2 < sz3 ? sz2 : sz3);

	glm::vec3 LLC(xmin, ymin, zmin);
	glm::vec3 URC(xmax, ymax, zmax);

	nerv::BVHbounds info{
		index,
		LLC,
		URC,
		glm::vec3(0.5f * LLC + 0.5f * URC)
	};

	return info;
}

glm::vec3 nerv::BVHbounds::offset(glm::vec3 point) const
{
	glm::vec3 o = point - LLC;

	if (URC.x > LLC.x) o.x /= URC.x - LLC.x;
	if (URC.y > LLC.y) o.y /= URC.y - LLC.y;
	if (URC.z > LLC.z) o.z /= URC.z - LLC.z;

	return o;
}

float nerv::BVHbounds::surfaceArea()
{
	glm::vec3  d = URC - LLC;
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

nerv::BVHAccel::BVHAccel(std::vector<nerv::primitive::triangle>& p, int maxPrimsInNode, splitMethod method) : maxPrimsInNode(std::min(255, maxPrimsInNode)), primitives(p), method(method)
{
	logger.startInit("BVH");
	if (primitives.size() == 0)
		return;

	std::vector<BVHbounds> primInfo(primitives.size());
	for (int i = 0; i < primitives.size(); i++)
	{
		primInfo[i] = BVHbound::triangleBoundingInfo(primitives[i], i);
	}
	logger.initLog("calculated all the primitives bounds");

	int totalNodes = 0;
	std::vector<primitive::triangle> orderedPrims;

	root;
	if (method == splitMethod::HLBVH)
		root;
	else
		root = recursiveBuild(primInfo, 0, primitives.size(), &totalNodes, orderedPrims);
	logger.initLog("BVH tree build");
	 
	primitives.swap(orderedPrims);
	logger.endInit();
}

nerv::BVHnode * nerv::BVHAccel::recursiveBuild(std::vector<BVHbound>& primInfo, int start, int end, int * totalNodes, std::vector<nerv::primitive::triangle>& orderedPrims)
{

	BVHnode * node = new BVHnode();

	//compute all the bounding volume of children primitives bounds
	BVHbound bounds;
	for (int i = start; i < end; i++)
	{
		bounds = BVHbound::uni(bounds, primInfo[i]);
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
			centroidBounds = BVHbound::uni(centroidBounds, primInfo[i].centroid);

		//get bounding volume diagonal and find the longest axis
		glm::vec diag = centroidBounds.URC - centroidBounds.LLC;
		int dim = 2;
		if (diag.x > diag.y && diag.x > diag.z)
			dim = 0;
		else if (diag.y > diag.z)
			dim = 1;

		int mid = (start + end) / 2;

		//if volume is null create the node and exit
		if (centroidBounds.URC[dim] == centroidBounds.LLC[dim])
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
			if (nPrimitives <= 4)
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
					buckets[b].bounds = BVHbound::uni(buckets[b].bounds, primInfo[i]);
				}

				float cost[nBucket - 1];
				for (int i = 0; i < nBucket - 1; i++)
				{
					BVHbound b0, b1;
					int count0, count1;
					count0 = count1 = 0;
					for (int j = 0; j <= i; j++)
					{
						b0 = BVHbound::uni(b0, buckets[j].bounds);
						count0 += buckets[j].count;
					}
					for (int j = i + 1; j < nBucket; j++)
					{
						b1 = BVHbound::uni(b1, buckets[j].bounds);
						count1 += buckets[j].count;
					}

					cost[i] = 0.125f + (count0 * b0.surfaceArea() + count1 * b1.surfaceArea()) / bounds.surfaceArea();
				}

				float minCost = cost[0];
				int minCostSplitBucket = 0;
				for (int i = 1; i < nBucket - 1; i++)
				{
					if (cost[0] < minCost)
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
