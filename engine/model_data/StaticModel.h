#pragma once
#include "ModelData.h"
#include "Rendering/Material.h"
#include "Physics/BoundingVolumes.h"

namespace ifr
{
	class StaticModel
	{
	public:
		StaticModel() {}

		StaticModel(const std::initializer_list<Ref<Mesh>>& meshes)
			: m_Meshes(meshes) {}

		StaticModel(const std::vector<Ref<Mesh>>& meshes)
			: m_Meshes(meshes) {}

		size_t GetMeshCount()		const { return m_Meshes.size(); }
		Ref<Mesh> GetMesh(size_t idx)	const { return m_Meshes.at(idx); }

		std::string Name = "";

		Ref<MaterialInstance> Material = nullptr;
		glm::vec4 Color = glm::vec4(1, 1, 1, 1);

		BoundingBoxData DefaultBoundingBoxData = BoundingBoxData();
		Ref<ConvexHull> DefaultConvexHull = nullptr;

	private:
		std::vector<Ref<Mesh>> m_Meshes;
	};
}
