#include "primitive.h"

nerv::primitive::bounds nerv::primitive::bounds::uni(nerv::primitive::bounds a, nerv::primitive::bounds b)
{
	nerv::primitive::bounds bound;
	bound.pMin = glm::min(a.pMin, b.pMin);
	bound.pMax = glm::max(a.pMax, b.pMax);
	return bound;
}

nerv::primitive::bounds nerv::primitive::bounds::uni(nerv::primitive::bounds a, glm::vec3 b)
{
	nerv::primitive::bounds bound;
	bound.pMin = glm::min(a.pMin, b);
	bound.pMax = glm::max(a.pMax, b);
	return bound;
}

nerv::primitive::bounds nerv::primitive::bounds::triangleBoundingInfo(nerv::primitive::triangle tris)
{
	glm::vec3 a(tris.v1[0], tris.v1[1], tris.v1[2]);
	glm::vec3 b(tris.v2[0], tris.v2[1], tris.v2[2]);
	glm::vec3 c(tris.v3[0], tris.v3[1], tris.v3[2]);

	glm::vec3 pMin = glm::min(a, b);
	glm::vec3 pMax = glm::max(a, b);
	pMin = glm::min(pMin, c);
	pMax = glm::max(pMax, c);

	bounds tmp = { pMin, pMax };

	return tmp;
}

float nerv::primitive::bounds::surfaceArea()
{
	glm::vec3  d = pMax - pMin;
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}
