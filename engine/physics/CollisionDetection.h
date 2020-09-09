#pragma once
#include "BoundingVolumes.h"
#include <model_data/ModelData.h>

namespace ifr
{
	namespace Physics
	{
		bool IsIntersecting(BoundingBox& box, const glm::vec3& ray, const glm::vec3& ray_position);
		bool IsIntersecting(BoundingBox& first, BoundingBox& second);
		bool IsIntersectingSAT(BoundingBox& first, BoundingBox& second, glm::vec3& mtv);
	}
}
