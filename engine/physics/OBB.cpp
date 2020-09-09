#include "OBB.h"

namespace ifr
{
	OBB::OBB(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix) : BoundingBox(xMax, xMin, yMax, yMin, zMax, zMin, correction_matrix)
	{
		m_AABB = Ref<BoundingBox>(new BoundingBox(xMax, xMin, yMax, yMin, zMax, zMin));
	}

	void OBB::UpdateAABB()
	{
		float xMaxNew = TopRightFar.x;
		float xMinNew = TopLeftFar.x;

		float yMaxNew = TopRightFar.y;
		float yMinNew = BottomRightFar.y;

		float zMaxNew = TopRightFar.z;
		float zMinNew = TopRightNear.z;

		for (glm::vec3 point : GetCorners())
		{
			if (point.x > xMaxNew) xMaxNew = point.x;
			if (point.x < xMinNew) xMinNew = point.x;

			if (point.y > yMaxNew) yMaxNew = point.y;
			if (point.y < yMinNew) yMinNew = point.y;

			if (point.z > zMaxNew) zMaxNew = point.z;
			if (point.z < zMinNew) zMinNew = point.z;
		}

		m_AABB->UpdateDimensions(xMaxNew, xMinNew, yMaxNew, yMinNew, zMaxNew, zMinNew);
	}
}
