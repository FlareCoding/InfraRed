#pragma once
#include "Rendering/RenderingContext.h"

#include <platform/window/DirectXWindow.h>
#include <platform/DirectXResources.h>

namespace ifr
{
	class DirectXContext : public RenderingContext
	{
	public:
		DirectXContext(WindowData& window_properties, HWND hwnd);
		~DirectXContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		void InitializeSwapChain();
		void InitializeRenderTargetView();
		void InitializeDepthStencilViewAndBuffer();
		void InitializeDepthStencilState();
		void InitializeViewPort();
		void InitializeRasterizerState();
		void InitializeBlendState();
		void InitializeSamplerState();
		void InitializeAudioEngine();
		void ReleaseAudioEngine();

	private:
		WindowData m_WindowProperties;
		HWND m_hwnd;

		ComPtr<IDXGISwapChain>			m_SwapChain;
		ComPtr<ID3D11Texture2D>			m_DepthStencilBuffer;
		ComPtr<ID3D11BlendState>		m_BlendState;

	private:
		std::string m_InitSuccessString = "";
	};
}
