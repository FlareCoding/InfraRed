#pragma once
#include <core/IFRCore.h>
#include <math/IFRMath.h>
#include <vector>

namespace ifr
{
	enum class TextureShaderLocation
	{
		DOMAIN_SHADER,
		PIXEL_SHADER
	};

	enum class TextureFormat
	{
		RGBA8,
		RGBA16,
	};

	class Texture
	{
	public:
		std::string Name = "";
		TextureShaderLocation ShaderLocation = TextureShaderLocation::PIXEL_SHADER;

		TextureFormat Format = TextureFormat::RGBA8;

		virtual ~Texture() = default;
		virtual uint32_t GetDimensions() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void* GetData() const = 0;

		virtual void WritePixel(uint32_t row, uint32_t column, const glm::ivec4& rgba) = 0;
		virtual glm::ivec4 ReadPixel(uint32_t row, uint32_t column) = 0;
		virtual void UpdateResourceData(void* data) = 0;
		virtual void UpdateResourceData() = 0;

		virtual void Bind(uint32_t slot) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual uint32_t GetDimensions() const override { return 2; };

		static Ref<Texture> LoadFromFile(const std::string& filepath, TextureFormat format = TextureFormat::RGBA8, bool flip_on_load = true);
		static Ref<Texture> CreateFromColor(const glm::vec3& rgb, TextureFormat format = TextureFormat::RGBA8);
		static Ref<Texture> CreateFromColor(const glm::vec3& rgb, uint32_t width, uint32_t height, TextureFormat format = TextureFormat::RGBA8);
	};

	class Texture3D : public Texture
	{
	public:
		virtual uint32_t GetDimensions() const override { return 3; };

		static Ref<Texture> LoadFromFiles(const std::vector<std::string>& filepaths);
	};
}
