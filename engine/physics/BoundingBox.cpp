#include "BoundingBox.h"
#include <model_data/ModelData.h>

namespace ifr
{
	BoundingBox::BoundingBox(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix)
	{
		m_MeshData = MakeRef<MeshData>();

		for (uint32_t i = 0; i < 8; i++)
			m_MeshData->Vertices.push_back(Vertex());

		m_MeshData->Indices = {
			0, 1, 5,
			0, 5, 4,

			2, 0, 4,
			2, 4, 6,

			2, 6, 3,
			3, 6, 7,

			3, 7, 1,
			1, 7, 3,

			2, 3, 0,
			0, 3, 1,

			6, 7, 4,
			4, 7, 5
		};

		UpdateDimensions(xMax, xMin, yMax, yMin, zMax, zMin, correction_matrix);
	}

	void BoundingBox::UpdateMesh()
	{
		m_MeshData->Vertices[0].position = { OriginalTopRightFar.x, OriginalTopRightFar.y, OriginalTopRightFar.z };
		m_MeshData->Vertices[1].position = { OriginalTopRightNear.x, OriginalTopRightNear.y, OriginalTopRightNear.z };
		m_MeshData->Vertices[2].position = { OriginalTopLeftFar.x, OriginalTopLeftFar.y, OriginalTopLeftFar.z };
		m_MeshData->Vertices[3].position = { OriginalTopLeftNear.x, OriginalTopLeftNear.y, OriginalTopLeftNear.z };

		m_MeshData->Vertices[4].position = { OriginalBottomRightFar.x, OriginalBottomRightFar.y, OriginalBottomRightFar.z };
		m_MeshData->Vertices[5].position = { OriginalBottomRightNear.x, OriginalBottomRightNear.y, OriginalBottomRightNear.z };
		m_MeshData->Vertices[6].position = { OriginalBottomLeftFar.x, OriginalBottomLeftFar.y, OriginalBottomLeftFar.z };
		m_MeshData->Vertices[7].position = { OriginalBottomLeftNear.x, OriginalBottomLeftNear.y, OriginalBottomLeftNear.z };

		if (m_Mesh == nullptr)
			m_Mesh = Mesh::Create(*m_MeshData);
		else
			m_Mesh->UpdateMeshData(m_MeshData->Vertices);
	}

	void BoundingBox::UpdateDimensions(float xMax, float xMin, float yMax, float yMin, float zMax, float zMin, const glm::mat4& correction_matrix)
	{
		this->xMax = xMax;
		this->xMin = xMin;
		this->yMax = yMax;
		this->yMin = yMin;
		this->zMax = zMax;
		this->zMin = zMin;

		OriginalTopRightFar		= correction_matrix * glm::vec4(xMax, yMax, zMax, 1);
		OriginalTopRightNear	= correction_matrix * glm::vec4(xMax, yMax, zMin, 1);
		OriginalTopLeftFar		= correction_matrix * glm::vec4(xMin, yMax, zMax, 1);
		OriginalTopLeftNear		= correction_matrix * glm::vec4(xMin, yMax, zMin, 1);

		OriginalBottomRightFar	= correction_matrix * glm::vec4(xMax, yMin, zMax, 1);
		OriginalBottomRightNear = correction_matrix * glm::vec4(xMax, yMin, zMin, 1);
		OriginalBottomLeftFar	= correction_matrix * glm::vec4(xMin, yMin, zMax, 1);
		OriginalBottomLeftNear	= correction_matrix * glm::vec4(xMin, yMin, zMin, 1);

		TopRightFar		= OriginalTopRightFar;
		TopRightNear	= OriginalTopRightNear;
		TopLeftFar		= OriginalTopLeftFar;
		TopLeftNear		= OriginalTopLeftNear;

		BottomRightFar	= OriginalBottomRightFar;
		BottomRightNear = OriginalBottomRightNear;
		BottomLeftFar	= OriginalBottomLeftFar;
		BottomLeftNear	= OriginalBottomLeftNear;

		SetTransform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	}

	void BoundingBox::UpdateDimensions(const glm::mat4& transform)
	{
		OriginalTopRightFar = transform * glm::vec4(xMax, yMax, zMax, 1);
		OriginalTopRightNear = transform * glm::vec4(xMax, yMax, zMin, 1);
		OriginalTopLeftFar = transform * glm::vec4(xMin, yMax, zMax, 1);
		OriginalTopLeftNear = transform * glm::vec4(xMin, yMax, zMin, 1);

		OriginalBottomRightFar = transform * glm::vec4(xMax, yMin, zMax, 1);
		OriginalBottomRightNear = transform * glm::vec4(xMax, yMin, zMin, 1);
		OriginalBottomLeftFar = transform * glm::vec4(xMin, yMin, zMax, 1);
		OriginalBottomLeftNear = transform * glm::vec4(xMin, yMin, zMin, 1);

		TopRightFar = OriginalTopRightFar;
		TopRightNear = OriginalTopRightNear;
		TopLeftFar = OriginalTopLeftFar;
		TopLeftNear = OriginalTopLeftNear;

		BottomRightFar = OriginalBottomRightFar;
		BottomRightNear = OriginalBottomRightNear;
		BottomLeftFar = OriginalBottomLeftFar;
		BottomLeftNear = OriginalBottomLeftNear;

		SetTransform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	}

	void BoundingBox::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		auto translation_matrix = glm::translate(glm::mat4(1.0f), position);

		auto rotation_matrix	= glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 });
		rotation_matrix			= glm::rotate(rotation_matrix, rotation.y, { 0, 1, 0 });
		rotation_matrix			= glm::rotate(rotation_matrix, rotation.z, { 0, 0, 1 });

		auto scaling_matrix = glm::scale(glm::mat4(1.0f), scale);

		auto transform = translation_matrix * rotation_matrix * scaling_matrix;
		
		TopRightFar		= transform		* OriginalTopRightFar;
		TopRightNear	= transform		* OriginalTopRightNear;
		TopLeftFar		= transform		* OriginalTopLeftFar;
		TopLeftNear		= transform		* OriginalTopLeftNear;

		BottomRightFar	= transform		* OriginalBottomRightFar;
		BottomRightNear = transform		* OriginalBottomRightNear;
		BottomLeftFar	= transform		* OriginalBottomLeftFar;
		BottomLeftNear	= transform		* OriginalBottomLeftNear;

		this->position	= position;
		this->rotation	= rotation;
		this->scale		= scale;

		m_RotationMatrix = rotation_matrix;
	}

	glm::vec3 BoundingBox::GetCenter()
	{
		return glm::vec3(
			(TopRightFar.x + TopLeftFar.x) / 2.0f,
			(TopRightFar.y + BottomRightFar.y) / 2.0f,
			(TopRightFar.z + TopRightNear.z) / 2.0f
		);
	}

	std::vector<glm::vec4> BoundingBox::GetCorners()
	{
		return 
		{
			TopRightFar, TopRightNear, TopLeftFar, TopLeftNear,
			BottomRightFar, BottomRightNear, BottomLeftFar, BottomLeftNear
		};
	}
	
	glm::vec3 BoundingBox::GetMaximums()
	{
		return glm::vec3(TopRightFar.x, TopRightFar.y, TopRightFar.z);
	}
	
	glm::vec3 BoundingBox::GetMinimums()
	{
		return glm::vec3(BottomLeftNear.x, BottomLeftNear.y, BottomLeftNear.z);
	}

	std::vector<glm::vec3> BoundingBox::GetLocalAxis()
	{
		auto x_axis = m_RotationMatrix * glm::vec4(1, 0, 0, 1);
		auto y_axis = m_RotationMatrix * glm::vec4(0, 1, 0, 1);
		auto z_axis = m_RotationMatrix * glm::vec4(0, 0, 1, 1);

		return
		{
			{ x_axis.x, x_axis.y, x_axis.z },
			{ y_axis.x, y_axis.y, y_axis.z},
			{ z_axis.x, z_axis.y, z_axis.z}
		};
	}

	glm::vec3 BoundingBox::GetHalfSizes()
	{
		return glm::vec3(
			(BottomRightFar.x - BottomLeftFar.x)	/ 2.0f,
			(BottomRightFar.y - TopRightFar.y)		/ 2.0f,
			(BottomRightFar.z - BottomRightNear.z)	/ 2.0f
		);
	}
}
