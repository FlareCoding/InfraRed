#include "DirectXContext.h"
#include "DirectXAdapterReader.h"
#include <logger/Logger.h>

namespace ifr
{
	DirectXContext::DirectXContext(WindowData& window_properties, HWND hwnd) : m_WindowProperties(window_properties), m_hwnd(hwnd)
	{
	}

	DirectXContext::~DirectXContext()
	{
		ReleaseAudioEngine();
	}

	void DirectXContext::Init()
	{
		InitializeSwapChain();
		InitializeRenderTargetView();
		InitializeDepthStencilViewAndBuffer();
		InitializeDepthStencilState();
		InitializeViewPort();
		InitializeRasterizerState();
		InitializeBlendState();
		InitializeSamplerState();
		InitializeAudioEngine();

		IFR_LOG_SUCCESS("Successfully Initialized DirectX");
		IFR_LOG_STATUS(m_InitSuccessString);
	}

	void DirectXContext::SwapBuffers()
	{
		m_SwapChain->Present(0, NULL);
	}

	void DirectXContext::InitializeSwapChain()
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		DXGI_SWAP_CHAIN_DESC swapchaindesc;
		ZeroMemory(&swapchaindesc, sizeof(swapchaindesc));

		swapchaindesc.BufferCount = 1;
		swapchaindesc.BufferDesc.Width = m_WindowProperties.width;
		swapchaindesc.BufferDesc.Height = m_WindowProperties.height;
		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchaindesc.BufferDesc.RefreshRate.Numerator = 144;
		swapchaindesc.BufferDesc.RefreshRate.Denominator = 1;
		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchaindesc.OutputWindow = m_hwnd;
		swapchaindesc.SampleDesc.Count = 1;
		swapchaindesc.SampleDesc.Quality = 0;
		swapchaindesc.Windowed = !m_WindowProperties.fullscreen;
		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapchaindesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT result = D3D11CreateDeviceAndSwapChain(adapters[0].m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, NULL, 0, D3D11_SDK_VERSION, &swapchaindesc, m_SwapChain.GetAddressOf(), DirectXResources::s_Device.GetAddressOf(), NULL, DirectXResources::s_DeviceContext.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create ID3D11Device and IDXGISwapChain");

		m_SwapChain->SetFullscreenState(m_WindowProperties.fullscreen, NULL);

		auto adapter_description = adapters[0].m_Description;
		char adapter_description_str[128];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, adapter_description.Description, -1, adapter_description_str, 128, &DefChar, NULL);

		m_InitSuccessString = std::string("\n========= DirectX Info =========\n  "
			"Renderer: " + std::string(adapter_description_str)) + "\n  "
			"Shared    System Memory: " +	std::to_string(adapter_description.SharedSystemMemory) + " bytes\n  "
			"Dedicated System Memory: " +   std::to_string(adapter_description.DedicatedVideoMemory) + " bytes\n  " 
			"Dedicated Video  Memory: " +	std::to_string(adapter_description.DedicatedSystemMemory) + " bytes\n";
	}

	void DirectXContext::InitializeRenderTargetView()
	{
		ComPtr<ID3D11Texture2D> back_buffer;
		HRESULT result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to get back buffer");

		result = DirectXResources::s_Device->CreateRenderTargetView(back_buffer.Get(), NULL, DirectXResources::s_RenderTargetView.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create RenderTargetView");
	}

	void DirectXContext::InitializeDepthStencilViewAndBuffer()
	{
		D3D11_TEXTURE2D_DESC depth_stencil_desc;
		depth_stencil_desc.Width = m_WindowProperties.width;
		depth_stencil_desc.Height = m_WindowProperties.height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
		depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.CPUAccessFlags = 0;
		depth_stencil_desc.MiscFlags = 0;

		HRESULT result = DirectXResources::s_Device->CreateTexture2D(&depth_stencil_desc, NULL, m_DepthStencilBuffer.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create depth stencil buffer");

		result = DirectXResources::s_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), NULL, DirectXResources::s_DepthStencilView.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create depth stencil view");

		DirectXResources::s_DeviceContext->OMSetRenderTargets(1, DirectXResources::s_RenderTargetView.GetAddressOf(), DirectXResources::s_DepthStencilView.Get());
	}

	void DirectXContext::InitializeDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc;
		ZeroMemory(&depth_stencil_state_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depth_stencil_state_desc.DepthEnable = true;
		depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		HRESULT result = DirectXResources::s_Device->CreateDepthStencilState(&depth_stencil_state_desc, DirectXResources::s_DepthStencilState.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create depth stencil state");
	}

	void DirectXContext::InitializeViewPort()
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		
		viewport.Width = (FLOAT)m_WindowProperties.width;
		viewport.Height = (FLOAT)m_WindowProperties.height;

		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DirectXResources::s_DeviceContext->RSSetViewports(1, &viewport);
	}

	void DirectXContext::InitializeRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc;
		ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerdesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerdesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		HRESULT result = DirectXResources::s_Device->CreateRasterizerState(&rasterizerdesc, DirectXResources::s_RasterizerState.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create rasterizer state");
	}

	void DirectXContext::InitializeBlendState()
	{
		D3D11_BLEND_DESC om_desc;
		ZeroMemory(&om_desc, sizeof(D3D11_BLEND_DESC));
		om_desc.RenderTarget[0].BlendEnable = true;
		om_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		om_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		om_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		om_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		om_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		om_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		om_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT result = DirectXResources::s_Device->CreateBlendState(&om_desc, m_BlendState.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create blend state");

		DirectXResources::s_DeviceContext->OMSetBlendState(m_BlendState.Get(), 0, 0xffffffff);
	}
	
	void DirectXContext::InitializeSamplerState()
	{
		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MaxAnisotropy = 0;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		
		HRESULT result = DirectXResources::s_Device->CreateSamplerState(&sampler_desc, DirectXResources::s_SamplerState.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create sampler state");
	}

	void DirectXContext::InitializeAudioEngine()
	{
		HRESULT result = XAudio2Create(DirectXResources::s_XAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create XAudio2 resource");

		result = DirectXResources::s_XAudio2->CreateMasteringVoice(&DirectXResources::s_XAudio2MasteringVoice);
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create XAudio2 mastering voice");
	}

	void DirectXContext::ReleaseAudioEngine()
	{
		if (DirectXResources::s_XAudio2MasteringVoice)
			DirectXResources::s_XAudio2MasteringVoice->DestroyVoice();
	}
}
