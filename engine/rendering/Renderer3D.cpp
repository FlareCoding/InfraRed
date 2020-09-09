#include "Renderer3D.h"
#include "Renderer.h"
#include <ecs/systems/RenderComponentSystem.h>

namespace ifr
{
	Renderer3D::SceneData Renderer3D::m_SceneData = { glm::mat4(1.0f), glm::mat4(1.0f) };

	Ref<Mesh> Renderer3D::m_Cube = nullptr;
	Ref<BasicShader> Renderer3D::m_Shader = nullptr;
	Ref<BasicAnimShader> Renderer3D::m_AnimShader = nullptr;
	Ref<Texture> Renderer3D::m_WhiteTexture = nullptr;

	void Renderer3D::Init()
	{
		m_Cube = Mesh::Create(Shapes::_3D::Cube);
		m_Shader = Shader::CreateDefaultBasicShader();
		m_AnimShader = Shader::CreateDefaultBasicAnimShader();
		m_WhiteTexture = Texture2D::CreateFromColor({ 255, 255, 255 });
	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::BeginScene(ICamera& camera, const Light& light)
	{
		m_SceneData.ProjectionMatrix = camera.GetProjection();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();
		m_SceneData.light = light;
		m_SceneData.CameraPosition = camera.GetPosition();

		m_Shader->Bind();
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawCube(const glm::vec3& position, float size, const glm::vec3& color)
	{
		m_WhiteTexture->Bind(0);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, size, size));

		m_Shader->SetMVPBuffer(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix);
		m_Shader->SetBasicShaderBuffer(color, 1.0f);
		m_Shader->SetSceneBuffers(m_SceneData.light, m_SceneData.CameraPosition);

		m_Cube->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_Cube->GetVertexArray());
	}

	void Renderer3D::DrawMesh(
		const Ref<Mesh>& mesh, 
		const glm::vec3& position, 
		const glm::vec3& rotation, 
		Ref<Texture> texture, 
		const glm::vec3& color, 
		float opacity, 
		glm::vec3 scale
	)
	{
		if (texture)
			texture->Bind(0);
		else
			m_WhiteTexture->Bind(0);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, scale);

		transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));

		m_Shader->SetMVPBuffer(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix);
		m_Shader->SetBasicShaderBuffer(color, opacity);
		m_Shader->SetSceneBuffers(m_SceneData.light, m_SceneData.CameraPosition);

		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer3D::DrawRawMeshData(const Ref<Mesh>& mesh)
	{
		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer3D::DrawRawVertexArrayData(const Ref<VertexArray>& va)
	{
		va->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(va);
	}

	void Renderer3D::DrawRawVertexArrayDataInstanced(const Ref<VertexArray>& va, uint32_t instance_count)
	{
		va->Bind();
		Renderer::s_RenderingAPI->DrawInstanced(va, instance_count);
	}

	void Renderer3D::DrawRawMeshDataControlPointPatchList(const Ref<Mesh>& mesh)
	{
		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexedControlPointPatchList(mesh->GetVertexArray());
	}

	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<MaterialInstance>& material, const glm::vec4& clip_plane)
	{
		if (!material)
		{
			DrawMesh(mesh, position, rotation, m_WhiteTexture, { 1, 1, 1 }, 1.0f, scale);
			return;
		}

		for (uint32_t i = 0; i < material->GetTextureCount(); i++)
		{
			auto texture = material->GetTexture(i);
			if (texture) texture->Bind(i);
		}

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, scale);

		transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));

		material->Bind();
		material->SetObjectSceneProperties(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix, m_SceneData.CameraPosition, m_SceneData.light, clip_plane);

		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(mesh->GetVertexArray());

		material->Unbind();
	}

	void Renderer3D::DrawAnimatedMesh(const Ref<Mesh>& mesh, std::vector<glm::mat4>& bone_transforms, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Ref<MaterialInstance>& material, const glm::vec4& clip_plane)
	{
		if (!material)
		{
			DrawAnimatedMesh(mesh, bone_transforms, position, rotation, m_WhiteTexture, { 1, 1, 1 }, 1.0f, scale);
			return;
		}

		for (uint32_t i = 0; i < material->GetTextureCount(); i++)
		{
			auto texture = material->GetTexture(i);
			if (texture) texture->Bind(i);
		}

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, scale);

		transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));

		material->Bind();
		material->SetObjectSceneProperties(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix, m_SceneData.CameraPosition, m_SceneData.light, bone_transforms, clip_plane);

		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer3D::DrawAnimatedMesh(
		const Ref<Mesh>& mesh, 
		std::vector<glm::mat4>& bone_transforms, 
		const glm::vec3& position, 
		const glm::vec3& rotation, 
		Ref<Texture> texture, 
		glm::vec3 color,
		float opacity,
		glm::vec3 scale
	)
	{
		texture->Bind(0);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, scale);

		transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));

		m_AnimShader->SetMVPBuffer(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix);
		m_AnimShader->SetBasicShaderBuffer(color, opacity);
		m_AnimShader->SetSceneBuffers(m_SceneData.light, m_SceneData.CameraPosition);
		m_AnimShader->SetBoneTransformsBuffer(bone_transforms);

		mesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(mesh->GetVertexArray());
	}
	
	void Renderer3D::BindBasicShader()
	{
		m_Shader->Bind();
	}
	
	void Renderer3D::BindBasicAnimShader()
	{
		m_AnimShader->Bind();
	}
}
