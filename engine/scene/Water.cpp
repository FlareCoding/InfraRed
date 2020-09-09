#include "Water.h"
#include <rendering/Renderer3D.h>
#include <rendering/Renderer.h>
#include <window/Window.h>

namespace ifr
{
	Ref<WaterChunk> WaterChunk::Create()
	{
		return Ref<WaterChunk>(new WaterChunk());
	}

	WaterChunk::WaterChunk()
	{
		m_WaterPlane = Mesh::Create(Shapes::_3D::Plane);
		m_Shader = Shader::CreateDefaultWaterShader();

		m_ReflectionBuffer = FrameBuffer::Create(s_ReflectionWidth, s_ReflectionHeight);
		m_RefractionBuffer = FrameBuffer::Create(s_RefractionWidth, s_RefractionHeight);

		DistortionMap	= Texture2D::LoadFromFile("assets\\textures\\WaterDistortionMap.png");
		NormalMap		= Texture2D::LoadFromFile("assets\\textures\\WaterNormalMap.png");
	}

	void WaterChunk::Render(ICamera& camera, const Light& light)
	{
		m_MoveFactor += WaveSpeed / 1000000.0f * (float)Time::GetElapsedTime();
		
		m_ReflectionBuffer->GetColorAttachmentTexture()->Bind(0);
		m_RefractionBuffer->GetColorAttachmentTexture()->Bind(1);
		DistortionMap->Bind(2);
		NormalMap->Bind(3);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, Position);
		transform = glm::scale(transform, { Size, Size, Size });
		transform = glm::rotate(transform, Rotation, glm::vec3(0, 1, 0));

		m_Shader->Bind();
		m_Shader->SetMVPBuffer(transform, camera.GetProjection(), camera.GetViewMatrix());
		m_Shader->SetSceneBuffers(light, camera.GetPosition(), Tiling);
		m_Shader->SetWaterBuffer(WaveStrength, m_MoveFactor);

		Renderer3D::DrawRawMeshData(m_WaterPlane);

		m_Shader->Unbind();
	}

	void WaterChunk::BindReflectionBuffer()
	{
		m_ReflectionBuffer->Bind();
		m_ReflectionBuffer->ClearBufferColor({ 1, 1, 1, 1 });
		Renderer::SetViewport(0, 0, s_ReflectionWidth, s_ReflectionHeight);
	}

	void WaterChunk::BindRefractionBuffer()
	{
		m_RefractionBuffer->Bind();
		m_RefractionBuffer->ClearBufferColor({ 1, 1, 1, 1 });
		Renderer::SetViewport(0, 0, s_RefractionWidth, s_RefractionHeight);
	}

	void WaterChunk::RestoreMainBackBuffer(uint32_t offsetx, uint32_t offsety, uint32_t width, uint32_t height)
	{
		m_ReflectionBuffer->RestoreMainBackBuffer();
		Renderer::SetViewport(offsetx, offsety, width, height);
	}
}
