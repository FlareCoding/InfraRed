#pragma once
#include <core/ifrcore.h>
#include "Rendering/VertexData.h"
#include "Rendering/VertexArray.h"
#include <map>
#include <functional>
#include "AnimationData.h"
#include "Physics/BoundingBox.h"

namespace ifr
{
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<int>	Indices;
	};

	class Mesh
	{
	public:
		static Ref<Mesh> Create(const MeshData& data);

		MeshData& GetMeshData() { return m_MeshData; }
		void UpdateMeshData(std::vector<Vertex>& data);
		void UpdateMeshData();

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		Mesh(const MeshData& data)
			: m_MeshData(data) {}

		MeshData m_MeshData;

	private:
		Ref<VertexArray> m_VertexArray;
	};

	class Bone
	{
	public:
		std::string Name;
		uint32_t ID = -1;

		std::vector<Bone> Children;

		glm::mat4 OffsetMatrix = glm::mat4(1.0f);
		glm::mat4 UserTransformation = glm::mat4(1.0f);
		glm::mat4 FinalTransform = glm::mat4(1.0f);

		std::vector<Keyframe> Keyframes;

		Bone() = default;
	};
}
