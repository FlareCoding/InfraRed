#pragma once
#include <model_data/ModelData.h>

namespace ifr
{
	class ConvexHull
	{
	public:
		static Ref<ConvexHull> Construct(const std::vector<Vertex>& data);

	private:
		ConvexHull(const std::vector<glm::vec4>& positions, const std::vector<int>& indices);

		Ref<MeshData> m_MeshData;
		Ref<Mesh> m_Mesh = nullptr;

		glm::mat4 m_RotationMatrix = glm::mat4(1.0f);
	};
}
