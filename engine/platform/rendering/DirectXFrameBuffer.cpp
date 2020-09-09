#include "DirectXFrameBuffer.h"
#include "DirectXTexture.h"

namespace ifr
{
	DirectXFrameBuffer::DirectXFrameBuffer(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		D3D11_TEXTURE2D_DESC DepthStencilDesc;
		ZeroMemory(&DepthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		TextureDesc.Width = m_Width;
		TextureDesc.Height = m_Height;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		TextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D> texture_buffer;
		HRESULT result = DirectXResources::s_Device->CreateTexture2D(&TextureDesc, NULL, texture_buffer.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("FrameBuffer> Failed to create ID3D11Texture2D texture buffer | HRESULT: " + std::to_string(result));
			return;
		}

		RTVDesc.Format = TextureDesc.Format;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;

		result = DirectXResources::s_Device->CreateRenderTargetView(texture_buffer.Get(), &RTVDesc, m_RenderTargetView.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("FrameBuffer> Failed to create ID3D11RenderTargetView | HRESULT: " + std::to_string(result));
			return;
		}

		SRVDesc.Format = TextureDesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;

		result = DirectXResources::s_Device->CreateShaderResourceView(texture_buffer.Get(), &SRVDesc, m_ShaderResource.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("FrameBuffer> Failed to create ID3D11ShaderResourceView | HRESULT: " + std::to_string(result));
			return;
		}

		DepthStencilDesc.Width = m_Width;
		DepthStencilDesc.Height = m_Height;
		DepthStencilDesc.MipLevels = 1;
		DepthStencilDesc.ArraySize = 1;
		DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilDesc.SampleDesc.Count = 1;
		DepthStencilDesc.SampleDesc.Quality = 0;
		DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilDesc.CPUAccessFlags = 0;
		DepthStencilDesc.MiscFlags = 0;
		
		ComPtr<ID3D11Texture2D> depth_buffer;
		result = DirectXResources::s_Device->CreateTexture2D(&DepthStencilDesc, NULL, depth_buffer.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("FrameBuffer> Failed to create depth stencil buffer");
			return;
		}

		result = DirectXResources::s_Device->CreateDepthStencilView(depth_buffer.Get(), NULL, m_DepthStencilView.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("FrameBuffer> Failed to create depth stencil view");
			return;
		}

		m_ColorAttachmentTexture = Ref<Texture>(new DirectXTexture2D(m_Width, m_Height, m_ShaderResource, TextureFormat::RGBA8));
		if (!m_ColorAttachmentTexture)
			IFR_LOG_ERROR("FrameBuffer> Failed to create color attachment texture");
	}
	
	uint32_t DirectXFrameBuffer::GetWidth() const
	{
		return m_Width;
	}
	
	uint32_t DirectXFrameBuffer::GetHeight() const
	{
		return m_Height;
	}
	
	void DirectXFrameBuffer::Bind() const
	{
		DirectXResources::s_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	}
	
	void DirectXFrameBuffer::RestoreMainBackBuffer() const
	{
		DirectXResources::s_DeviceContext->OMSetRenderTargets(1, DirectXResources::s_RenderTargetView.GetAddressOf(), DirectXResources::s_DepthStencilView.Get());
	}
	
	Ref<Texture> DirectXFrameBuffer::GetColorAttachmentTexture() const
	{
		return m_ColorAttachmentTexture;
	}
	
	void DirectXFrameBuffer::ClearBufferColor(const glm::vec4& color) const
	{
		float bgcolor[] = { color.r, color.g, color.b, color.a };
		DirectXResources::s_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), bgcolor);
		DirectXResources::s_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}
