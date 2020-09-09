#pragma once
#include <rendering/FrameBuffer.h>
#include <platform/DirectXResources.h>

namespace ifr
{
	class DirectXFrameBuffer : public FrameBuffer
	{
	public:
		DirectXFrameBuffer(uint32_t width, uint32_t height);

		// Inherited via FrameBuffer
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void Bind() const override;
		virtual void RestoreMainBackBuffer() const override;

		virtual void ClearBufferColor(const glm::vec4& color) const override;

		virtual Ref<Texture> GetColorAttachmentTexture() const override;

	private:
		uint32_t m_Width, m_Height;

		Ref<Texture> m_ColorAttachmentTexture = nullptr;

	private:
		ComPtr<ID3D11RenderTargetView>   m_RenderTargetView;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResource;
		ComPtr<ID3D11DepthStencilView>   m_DepthStencilView;
	};
}
