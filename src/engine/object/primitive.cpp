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
	glm::vec3 a(tris.v1[0], tris.v1[1], tris.v1[2]);
	glm::vec3 b(tris.v2[0], tris.v2[1], tris.v2[2]);
	glm::vec3 c(tris.v3[0], tris.v3[1], tris.v3[2]);

	glm::vec3 llc(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	glm::vec3 urc(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));

	bounds tmp = { llc, urc };

	return uni(tmp, c);
}

float nerv::primitive::bounds::surfaceArea()
{
	glm::vec3  d = URC - LLC;
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}
