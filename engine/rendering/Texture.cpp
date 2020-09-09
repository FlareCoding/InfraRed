#include "Texture.h"
#include "Renderer.h"
#include <platform/rendering/DirectXTexture.h>

namespace ifr
{
	Ref<Texture> Texture2D::LoadFromFile(const std::string& filepath, TextureFormat format, bool flip_on_load)
	{
		return Ref<Texture>(DirectXTexture2D::LoadFromFile(filepath, format, flip_on_load));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3& rgb, TextureFormat format)
	{
		return Ref<Texture>(DirectXTexture2D::CreateFromColor(rgb, format));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3& rgb, uint32_t width, uint32_t height, TextureFormat format)
	{
		return Ref<Texture>(DirectXTexture2D::CreateFromColor(rgb, width, height, format));
	}
	
	Ref<Texture> Texture3D::LoadFromFiles(const std::vector<std::string>& filepaths)
	{
		return Ref<Texture>(DirectXTexture3D::LoadFromFiles(filepaths));
	}
}
