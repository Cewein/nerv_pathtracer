#include "primitive.h"

bool nerv::sphere::boundingBox(nerv::AABB * out)
{
	out->min = glm::vec3(pos) - glm::vec3(pos.w);
	out->max = glm::vec3(pos) + glm::vec3(pos.w);
	return true;
}

