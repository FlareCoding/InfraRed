#include "CollisionDetection.h"

namespace ifr
{
	namespace Physics
	{
		bool SATtest(BoundingBox& first, BoundingBox& second, glm::vec3 axis, float& depth);

		bool IsIntersecting(BoundingBox& box, const glm::vec3& ray, const glm::vec3& ray_position)
		{
			auto inv_ray = 1.0f / ray;
			auto min = box.GetMinimums();
			auto max = box.GetMaximums();

			float t1 = (min.x - ray_position.x) * inv_ray.x;
			float t2 = (max.x - ray_position.x) * inv_ray.x;
			float t3 = (min.y - ray_position.y) * inv_ray.y;
			float t4 = (max.y - ray_position.y) * inv_ray.y;
			float t5 = (min.z - ray_position.z) * inv_ray.z;
			float t6 = (max.z - ray_position.z) * inv_ray.z;

			float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
			float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));
			
			if (tmax < 0) 
				return false;

			if (tmin > tmax)
				return false;

			return true;
		}

		bool IsIntersecting(BoundingBox& first, BoundingBox& second)
		{
			auto first_mins = first.GetMinimums();
			auto first_maxes = first.GetMaximums();

			auto second_mins = second.GetMinimums();
			auto second_maxes = second.GetMaximums();

			bool xAxis = first_mins.x <= second_maxes.x && first_maxes.x >= second_mins.x;
			bool yAxis = first_mins.y <= second_maxes.y && first_maxes.y >= second_mins.y;
			bool zAxis = first_mins.z <= second_maxes.z && first_maxes.z >= second_mins.z;

			return xAxis && yAxis && zAxis;
		}

		bool IsIntersectingSAT(BoundingBox& first, BoundingBox& second, glm::vec3& mtv)
		{
			auto local_axis_first	= first.GetLocalAxis();
			auto local_axis_second	= second.GetLocalAxis();

			auto a0 = local_axis_first[0];
			auto a1 = local_axis_first[1];
			auto a2 = local_axis_first[2];

			auto b0 = local_axis_second[0];
			auto b1 = local_axis_second[1];
			auto b2 = local_axis_second[2];

			std::vector<glm::vec3> axis_list =
			{
				a0, a1, a2,
				b0, b1, b2,

				glm::cross(a0, b0),
				glm::cross(a0, b1),
				glm::cross(a0, b2),

				glm::cross(a1, b0),
				glm::cross(a1, b1),
				glm::cross(a1, b2),

				glm::cross(a2, b0),
				glm::cross(a2, b1),
				glm::cross(a2, b2),
			};

			glm::vec3 mtv_axis;
			float penetration_depth = (float)HUGE;

			for (auto& axis : axis_list)
			{
				float depth = 0;
				if (SATtest(first, second, axis, depth))
					return false;

				if (glm::length(axis) > 0.1f)
				{
					if (depth < penetration_depth)
					{
						penetration_depth = depth;
						mtv_axis = axis;
					}
				}
			}

			mtv = mtv_axis * penetration_depth;
			return true;
		}
	
		bool SATtest(BoundingBox& first, BoundingBox& second, glm::vec3 axis, float& depth)
		{
			if (axis.x == 0 && axis.y == 0 && axis.z == 0) return false;

			float aMin = (float)HUGE;
			float aMax = (float)-HUGE;
			float bMin = (float)HUGE;
			float bMax = (float)-HUGE;

			auto corners_a = first.GetCorners();
			auto corners_b = second.GetCorners();

			for (uint32_t i = 0; i < corners_a.size(); i++)
			{
				auto aDist = glm::dot({ corners_a[i].x, corners_a[i].y, corners_a[i].z }, axis);
				aMin = aDist < aMin ? aDist : aMin;
				aMax = aDist > aMax ? aDist : aMax;

				auto bDist = glm::dot({ corners_b[i].x, corners_b[i].y, corners_b[i].z }, axis);
				bMin = bDist < bMin ? bDist : bMin;
				bMax = bDist > bMax ? bDist : bMax;
			}

			// One-dimensional intersection test between a and b
			auto longSpan = glm::max(aMax, bMax) - glm::min(aMin, bMin);
			auto sumSpan = aMax - aMin + bMax - bMin;

			depth = sumSpan - longSpan;

			return longSpan >= sumSpan; // > to treat touching as intersection
		}
	}
}
