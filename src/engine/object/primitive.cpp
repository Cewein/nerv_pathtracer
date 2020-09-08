#include "primitive.h"

nerv::primitive::bounds nerv::primitive::bounds::uni(nerv::primitive::bounds a, nerv::primitive::bounds b)
{
	nerv::primitive::bounds bound;
	bound.LLC = glm::vec3(std::min(a.LLC.x, b.LLC.x), std::min(a.LLC.y, b.LLC.y), std::min(a.LLC.z, b.LLC.z));
	bound.URC = glm::vec3(std::max(a.URC.x, b.URC.x), std::max(a.URC.y, b.URC.y), std::max(a.URC.z, b.URC.z));
	return bound;
}

nerv::primitive::bounds nerv::primitive::bounds::uni(nerv::primitive::bounds a, glm::vec3 b)
{
	nerv::primitive::bounds bound;
	bound.LLC = glm::vec3(std::min(a.LLC.x, b.x), std::min(a.LLC.y, b.y), std::min(a.LLC.z, b.z));
	bound.URC = glm::vec3(std::max(a.URC.x, b.x), std::max(a.URC.y, b.y), std::max(a.URC.z, b.z));
	return bound;
}

nerv::primitive::bounds nerv::primitive::bounds::triangleBoundingInfo(nerv::primitive::triangle tris)
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

	nerv::primitive::bounds info{
		LLC,
		URC
	};

	return info;
}

float nerv::primitive::bounds::surfaceArea()
{
	glm::vec3  d = URC - LLC;
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}
