#include "primitive.h"

nerv::primitive::aabbs nerv::primitive::aabbs::uni(nerv::primitive::aabbs a, nerv::primitive::aabbs b)
{
	nerv::primitive::aabbs bound;
	bound.pMin = glm::min(a.pMin, b.pMin);
	bound.pMax = glm::max(a.pMax, b.pMax);
	return bound;
}

nerv::primitive::aabbs nerv::primitive::aabbs::uni(nerv::primitive::aabbs a, glm::vec3 b)
{
	nerv::primitive::aabbs bound;
	bound.pMin = glm::min(a.pMin, b);
	bound.pMax = glm::max(a.pMax, b);
	return bound;
}

glm::vec3 nerv::primitive::aabbs::offset(glm::vec3 point) const
{
	glm::vec3 o = point - pMin;

	if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
	if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
	if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;

	return o;
}

nerv::primitive::aabbs nerv::primitive::aabbs::triangleBoundingInfo(nerv::primitive::triangle tris)
{
	glm::vec3 a(tris.v1[0], tris.v1[1], tris.v1[2]);
	glm::vec3 b(tris.v2[0], tris.v2[1], tris.v2[2]);
	glm::vec3 c(tris.v3[0], tris.v3[1], tris.v3[2]);

	glm::vec3 pMin = glm::min(a, b);
	glm::vec3 pMax = glm::max(a, b);
	pMin = glm::min(pMin, c);
	pMax = glm::max(pMax, c);

	aabbs tmp = { pMin, pMax };

	return tmp;
}

float nerv::primitive::aabbs::surfaceArea()
{
	glm::vec3  d = pMax - pMin;
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}
