#pragma once
#include <rendering/Texture.h>
#include <platform/DirectXResources.h>

namespace ifr
{
	class DirectXTexture2D : public Texture2D
	{
		friend class DirectXFrameBuffer;

	public:
		static DirectXTexture2D* LoadFromFile(const std::string& filepath, TextureFormat format = TextureFormat::RGBA8, bool flip_on_load = true);
		static DirectXTexture2D* CreateFromColor(const glm::vec3& rgb, TextureFormat format = TextureFormat::RGBA8);
		static DirectXTexture2D* CreateFromColor(const glm::vec3& rgb, uint32_t width, uint32_t height, TextureFormat format = TextureFormat::RGBA8);

		DirectXTexture2D(void* img_data, uint32_t width, uint32_t height, TextureFormat format);
		~DirectXTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual void* GetData() const override;

		// Inherited via Texture
		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void* data) override;
		virtual void WritePixel(uint32_t row, uint32_t column, const glm::ivec4& rgba) override;
		virtual glm::ivec4 ReadPixel(uint32_t row, uint32_t column) override;

		virtual void Bind(uint32_t slot) const override;

	private:
		void* m_ImageData = nullptr;
		uint32_t m_Width = 0, m_Height = 0;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResource;
		ComPtr<ID3D11Texture2D> m_TextureBuffer;

	private:
		// Special constructor that is used only by frame buffers;
		DirectXTexture2D(uint32_t width, uint32_t height, const ComPtr<ID3D11ShaderResourceView>& SRV, TextureFormat format);
	};

	class DirectXTexture3D : public Texture3D
	{
	public:
		static DirectXTexture3D* LoadFromFiles(const std::vector<std::string>& filepaths);

		DirectXTexture3D(std::vector<unsigned char*> texture_data, uint32_t width, uint32_t height);
		~DirectXTexture3D();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual void* GetData() const override;

		virtual void Bind(uint32_t slot) const override;

	private:
		std::vector<unsigned char*> m_ImageData;
		uint32_t m_Width = 0, m_Height = 0;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResource;

		// Inherited via Texture3D
		virtual void WritePixel(uint32_t row, uint32_t column, const glm::ivec4& rgba) override {}
		virtual void UpdateResourceData(void* data) override {}
		virtual void UpdateResourceData() override {}
		virtual glm::ivec4 ReadPixel(uint32_t row, uint32_t column) override { return { 0, 0, 0, 0 }; }
	};
}
