#include "primitive.h"

bool nerv::sphere::boundingBox(nerv::AABB * out)
{
	out->min = glm::vec3(pos) - glm::vec3(pos.w);
	out->max = glm::vec3(pos) + glm::vec3(pos.w);
	return true;
}

bool nerv::triangle::boundingBox(nerv::AABB * out)
{
	out->min = glm::min(v1.xyz(), glm::min(v2.xyz(), v3.xyz()));
	out->max = glm::max(v1.xyz(), glm::max(v2.xyz(), v3.xyz()));

	return true;
}
