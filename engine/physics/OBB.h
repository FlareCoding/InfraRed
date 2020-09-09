#pragma once
#include "BoundingBox.h"

namespace ifr
{
	class OBB : public BoundingBox
	{
	public:
		OBB(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix = glm::mat4(1.0f));

		Ref<BoundingBox> GetAABB() { return m_AABB; }
		void UpdateAABB();

	private:
		Ref<BoundingBox> m_AABB = nullptr;
	};
}
