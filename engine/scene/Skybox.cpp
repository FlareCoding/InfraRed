#include "Skybox.h"
#include <model_data/shapes/shapes.h>
#include <rendering/Renderer3D.h>

namespace ifr
{
	static constexpr uint32_t SKYBOX_SIZE = 5000;

	Ref<Skybox> Skybox::Create()
	{
		return Ref<Skybox>(new Skybox());
	}

	Skybox::Skybox()
	{
		MeshData cube_data = Shapes::_3D::Cube;

		for (auto& vertex : cube_data.Vertices)
			vertex.position *= (float)SKYBOX_SIZE;

		m_Mesh = Mesh::Create(cube_data);

		m_Shader = Shader::CreateDefaultSkyboxShader();
	}

	void Skybox::Render(ICamera& camera)
	{
		if (!texture)
		{
			IFR_LOG_ERROR("Skybox>  [-] Failed to Render | Missing Texture [-]");
			return;
		}

		texture->Bind(0);

		m_Shader->Bind();

		auto view_matrix = camera.GetViewMatrix();
		auto frame_rotation_value = RotationSpeed / 10000.0f * (float)Time::GetTimestep();
		m_Rotation += frame_rotation_value;
		view_matrix = glm::rotate(view_matrix, glm::radians(m_Rotation), { 0, 1, 0 });

		m_Shader->SetSkyboxBuffer(camera.GetProjection(), view_matrix);

		Renderer3D::DrawRawMeshData(m_Mesh);

		m_Shader->Unbind();
	}
}
