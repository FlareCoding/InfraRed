#include "Renderer.h"
#include <platform/rendering/DirectXRenderingAPI.h>
#include <ecs/systems/RenderComponentSystem.h>

namespace ifr
{
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<RenderingAPI>(new DirectXRenderingAPI());

	void Renderer::ClearScreenColor(const glm::vec4& color)
	{
		s_RenderingAPI->ClearScreenColor(color);
	}

	void Renderer::ClearScreenBuffers()
	{
		s_RenderingAPI->ClearScreenBuffers();
	}

	void Renderer::SetWireframe(bool wf)
	{
		s_RenderingAPI->SetWireframe(wf);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RenderingAPI->SetViewport(x, y, width, height);
	}

	void Renderer::Init(Window* window)
	{
		Renderer2D::Init();
		Renderer3D::Init();
		ImGuiRenderer::Init(window);
		RenderComponentSystem::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();
		ImGuiRenderer::Shutdown();
	}
}
