#include "ConvexHull.h"

namespace ifr
{
	Ref<ConvexHull> ConvexHull::Construct(const std::vector<Vertex>& data)
	{
		return 0;
	}

	ConvexHull::ConvexHull(const std::vector<glm::vec4>& positions, const std::vector<int>& indices)
	{
		m_MeshData = MakeRef<MeshData>();

		for (uint32_t i = 0; i < positions.size(); i++)
		{
			Vertex v;
			v.position = { positions[i].x, positions[i].y, positions[i].z };
			m_MeshData->Vertices.push_back(v);
		}

		m_MeshData->Indices = indices;
	}
}
