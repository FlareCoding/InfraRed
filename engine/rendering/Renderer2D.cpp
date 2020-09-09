#include "Renderer2D.h"
#include "Renderer.h"
#include <window/Window.h>

namespace ifr
{
	Renderer2D::SceneData Renderer2D::m_SceneData = { glm::mat4(1.0f), glm::mat4(1.0f) };
	
	Ref<Mesh> Renderer2D::m_Quad				= nullptr;
	Ref<Mesh> Renderer2D::m_Triangle			= nullptr;
	Ref<BasicGUIShader> Renderer2D::m_Shader	= nullptr;
	Ref<Texture> Renderer2D::m_WhiteTexture		= nullptr;

	void Renderer2D::Init()
	{	
		m_Quad = Mesh::Create(Shapes::_2D::Quad);
		m_Triangle = Mesh::Create(Shapes::_2D::Triangle);

		m_Shader = Shader::CreateDefaultBasicGUIShader();
		m_WhiteTexture = Texture2D::CreateFromColor({ 255, 255, 255 });
	}

	void Renderer2D::Shutdown()
	{
	}
	
	void Renderer2D::BeginScene(ICamera& camera)
	{
		m_SceneData.ProjectionMatrix = camera.GetProjection();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();

		m_Shader->Bind();
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float opacity)
	{
		DrawQuad({ position.x, position.y, -100000000.0f }, size, color, opacity);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float opacity)
	{
		m_WhiteTexture->Bind(0);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

		m_Shader->SetMVPBuffer(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix);
		m_Shader->SetBasicShaderBuffer(color, opacity);

		m_Quad->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_Quad->GetVertexArray());
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float opacity, float tiling)
	{
		DrawQuad({ position.x, position.y, -100000000.0f }, size, texture, opacity, tiling);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float opacity, float tiling)
	{
		texture->Bind(0);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

		m_Shader->SetMVPBuffer(transform, m_SceneData.ProjectionMatrix, m_SceneData.ViewMatrix);
		m_Shader->SetBasicShaderBuffer(glm::vec3(1, 1, 1), opacity, tiling);

		m_Quad->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_Quad->GetVertexArray());
	}
}
