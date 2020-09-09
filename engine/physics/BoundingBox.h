#pragma once
#include <math/IFRMath.h>
#include <vector>

namespace ifr
{
	class Mesh;
	struct MeshData;

	struct BoundingBoxData
	{
		float xMax = 0, xMin = 0;
		float yMax = 0, yMin = 0;
		float zMax = 0, zMin = 0;
	};

	class BoundingBox
	{
	public:
		BoundingBox(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix = glm::mat4(1.0f));

		void UpdateMesh();
		void UpdateDimensions(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix = glm::mat4(1.0f));
		void UpdateDimensions(const glm::mat4& transform);
		Ref<Mesh> GetMesh() { return m_Mesh; }

		glm::vec3 position = { 0, 0, 0 };
		glm::vec3 rotation = { 0, 0, 0 };
		glm::vec3 scale = { 1, 1, 1 };

		void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		glm::vec3 GetCenter();
		glm::vec3 GetHalfSizes();

		std::vector<glm::vec4> GetCorners();
		glm::vec3 GetMaximums();
		glm::vec3 GetMinimums();

		std::vector<glm::vec3> GetLocalAxis();

		glm::mat4 GetRotation() { return m_RotationMatrix; }

		glm::vec4 TopRightFar;
		glm::vec4 TopRightNear;
		glm::vec4 TopLeftFar;
		glm::vec4 TopLeftNear;

		glm::vec4 BottomRightFar;
		glm::vec4 BottomRightNear;
		glm::vec4 BottomLeftFar;
		glm::vec4 BottomLeftNear;

		glm::vec3 DebugColor = glm::vec3(0, 1, 0);

	protected:
		Ref<MeshData> m_MeshData;
		Ref<Mesh> m_Mesh = nullptr;

		glm::vec4 OriginalTopRightFar;
		glm::vec4 OriginalTopRightNear;
		glm::vec4 OriginalTopLeftFar;
		glm::vec4 OriginalTopLeftNear;

		glm::vec4 OriginalBottomRightFar;
		glm::vec4 OriginalBottomRightNear;
		glm::vec4 OriginalBottomLeftFar;
		glm::vec4 OriginalBottomLeftNear;

		float xMin, xMax;
		float yMin, yMax;
		float zMin, zMax;

		glm::mat4 m_RotationMatrix = glm::mat4(1.0f);
	};
}
