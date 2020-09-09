#pragma once
#include "RenderingAPI.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "ImGuiRenderer.h"

namespace ifr
{
	class Window;

	class Renderer
	{
	public:
		static void ClearScreenColor(const glm::vec4& color);
		static void ClearScreenBuffers();

		static void SetWireframe(bool wf);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void Init(Window* window);
		static void Shutdown();

	private:
		static UniqueRef<RenderingAPI> s_RenderingAPI;

	private:
		friend class Renderer2D;
		friend class Renderer3D;
	};
}
