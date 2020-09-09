#include "DirectXTexture.h"
#include <stb_image/stb_image.h>
#include <logger/Logger.h>

namespace ifr
{
	DirectXTexture2D* DirectXTexture2D::LoadFromFile(const std::string& filepath, TextureFormat format, bool flip_on_load)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flip_on_load);

		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			IFR_LOG_ERROR("Texture2D> [-] Failed to load texture2D: " + filepath + " [-]");
			return nullptr;
		}

		DirectXTexture2D* instance = new DirectXTexture2D(data, width, height, format);
		instance->m_ImageData = data;

		IFR_LOG_SUCCESS("Texture2D>    [+] Loaded " + filepath + " [+]");

		return instance;
	}

	DirectXTexture2D* DirectXTexture2D::CreateFromColor(const glm::vec3& rgb, TextureFormat format)
	{
		DirectXTexture2D* instance;

		if (format == TextureFormat::RGBA8)
		{
			unsigned char data[4] = { (unsigned char)rgb.x, (unsigned char)rgb.y, (unsigned char)rgb.z, (unsigned char)255 };
			instance = new DirectXTexture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(4);
			memcpy_s(instance->m_ImageData, 4, data, 4);
		}
		else
		{
			uint16_t data[4] = { (uint16_t)rgb.x, (uint16_t)rgb.y, (uint16_t)rgb.z, (uint16_t)65535 };
			instance = new DirectXTexture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(8);
			memcpy_s(instance->m_ImageData, 8, data, 8);
		}

		return instance;
	}

	DirectXTexture2D* DirectXTexture2D::CreateFromColor(const glm::vec3& rgb, uint32_t width, uint32_t height, TextureFormat format)
	{
		DirectXTexture2D* instance;

#pragma warning( push )
#pragma warning( disable : 6386)
		if (format == TextureFormat::RGBA8)
		{
			unsigned char* data = new unsigned char[(size_t)width * (size_t)height * (size_t)4];

			for (size_t r = 0; r < (size_t)width * 4; r++)
			{
				for (size_t c = 0; c < (size_t)height; c += 4)
				{
					size_t idx = r * width + c;
					data[idx]		= (unsigned char)(unsigned int)rgb.r;
					data[idx + 1]	= (unsigned char)(unsigned int)rgb.g;
					data[idx + 2]	= (unsigned char)(unsigned int)rgb.b;
					data[idx + 3]	= (unsigned char)255;
				}
			}

			instance = new DirectXTexture2D(data, width, height, format);

			instance->m_ImageData = new unsigned char[(size_t)width * (size_t)height * (size_t)4];
			memcpy_s(instance->m_ImageData, ((size_t)width * (size_t)height * (size_t)4), data, ((size_t)width * (size_t)height * (size_t)4));

			delete[] data;
		}
		else
		{
			uint16_t* data = new uint16_t[(size_t)width * (size_t)height * (size_t)4];

			for (size_t r = 0; r < (size_t)width * 4; r++)
			{
				for (size_t c = 0; c < (size_t)height; c += 4)
				{
					size_t idx		= r * width + c;
					data[idx]		= (uint16_t)(unsigned int)rgb.r;
					data[idx + 1]	= (uint16_t)(unsigned int)rgb.g;
					data[idx + 2]	= (uint16_t)(unsigned int)rgb.b;
					data[idx + 3]	= (uint16_t)65535;
				}
			}

			instance = new DirectXTexture2D(data, width, height, format);

			instance->m_ImageData = new uint16_t[(size_t)width * (size_t)height * (size_t)4];
			memcpy_s(instance->m_ImageData, ((size_t)width * (size_t)height * (size_t)8), data, ((size_t)width * (size_t)height * (size_t)8));

			delete[] data;
		}
#pragma warning( pop )

		return instance;
	}

	DirectXTexture2D::DirectXTexture2D(void* img_data, uint32_t width, uint32_t height, TextureFormat format)
	{
		m_Width = width;
		m_Height = height;
		Format = format;

		DXGI_FORMAT dxgi_format;
		uint32_t byte_stride = (format == TextureFormat::RGBA16) ? 8 : 4;

		switch (format)
		{
		case TextureFormat::RGBA8:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case TextureFormat::RGBA16:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM;
			break;
		}
		default:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		}

		D3D11_SUBRESOURCE_DATA resource_data;
		ZeroMemory(&resource_data, sizeof(D3D11_SUBRESOURCE_DATA));
		resource_data.pSysMem = img_data;
		resource_data.SysMemPitch = byte_stride * width;
		resource_data.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
		texture_desc.Width = m_Width;
		texture_desc.Height = m_Height;
		texture_desc.ArraySize = 1;
		texture_desc.MipLevels = 1;
		texture_desc.Format = dxgi_format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DYNAMIC;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT result = DirectXResources::s_Device->CreateTexture2D(&texture_desc, &resource_data, m_TextureBuffer.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("Texture2D> Failed to create ID3D11Texture2D | HRESULT: " + std::to_string(result));
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = dxgi_format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		SRVDesc.Texture2D.MostDetailedMip = 0;

		result = DirectXResources::s_Device->CreateShaderResourceView(m_TextureBuffer.Get(),
			&SRVDesc, m_ShaderResource.GetAddressOf());

		if (FAILED(result))
			IFR_LOG_ERROR("Texture2D> Failed to create shader resource view");
	}

	DirectXTexture2D::~DirectXTexture2D()
	{
		stbi_image_free(m_ImageData);
	}

	void* DirectXTexture2D::GetData() const
	{
		return m_ImageData;
	}

	void DirectXTexture2D::Bind(uint32_t slot) const
	{
		if (ShaderLocation == TextureShaderLocation::PIXEL_SHADER) DirectXResources::s_DeviceContext->PSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
		if (ShaderLocation == TextureShaderLocation::DOMAIN_SHADER) DirectXResources::s_DeviceContext->DSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
	}

	DirectXTexture2D::DirectXTexture2D(uint32_t width, uint32_t height, const ComPtr<ID3D11ShaderResourceView>& SRV, TextureFormat format)
	{
		m_Width = width;
		m_Height = height;
		m_ShaderResource = SRV;
		Format = format;
	}

	void DirectXTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_ImageData);
	}

	void DirectXTexture2D::UpdateResourceData(void* data)
	{
		uint64_t byte_offset = (Format == TextureFormat::RGBA16) ? 8 : 4;

		D3D11_MAPPED_SUBRESOURCE resource;
		DirectXResources::s_DeviceContext->Map(m_TextureBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, data, (size_t)m_Width * (size_t)m_Height * byte_offset);
		DirectXResources::s_DeviceContext->Unmap(m_TextureBuffer.Get(), 0);
	}

	void DirectXTexture2D::WritePixel(uint32_t row, uint32_t column, const glm::ivec4& rgba)
	{
		uint64_t idx = (((uint64_t)row * (uint64_t)m_Width) + (uint64_t)column) * 4;

		if (idx >= (uint64_t)m_Width * (uint64_t)m_Height * 4 || idx < 0) return;

		if (Format == TextureFormat::RGBA8)
		{
			((unsigned char*)m_ImageData)[idx]		= (unsigned char)rgba.r;
			((unsigned char*)m_ImageData)[idx + 1]	= (unsigned char)rgba.g;
			((unsigned char*)m_ImageData)[idx + 2]	= (unsigned char)rgba.b;
			((unsigned char*)m_ImageData)[idx + 3]	= (unsigned char)rgba.a;
		}
		else
		{
			((uint16_t*)m_ImageData)[idx]		= (uint16_t)rgba.r;
			((uint16_t*)m_ImageData)[idx + 1]	= (uint16_t)rgba.g;
			((uint16_t*)m_ImageData)[idx + 2]	= (uint16_t)rgba.b;
			((uint16_t*)m_ImageData)[idx + 3]	= (uint16_t)rgba.a;
		}
	}

	glm::ivec4 DirectXTexture2D::ReadPixel(uint32_t row, uint32_t column)
	{
		uint64_t idx = (((uint64_t)row * (uint64_t)m_Width) + (uint64_t)column) * 4;

		if (idx >= (uint64_t)m_Width * (uint64_t)m_Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba;

		if (Format == TextureFormat::RGBA8)
		{
			rgba.r = (uint32_t)(((unsigned char*)m_ImageData)[idx]);
			rgba.g = (uint32_t)(((unsigned char*)m_ImageData)[idx + 1]);
			rgba.b = (uint32_t)(((unsigned char*)m_ImageData)[idx + 2]);
			rgba.a = (uint32_t)(((unsigned char*)m_ImageData)[idx + 3]);
		}
		else
		{
			rgba.r = (uint32_t)(((uint16_t*)m_ImageData)[idx]);
			rgba.g = (uint32_t)(((uint16_t*)m_ImageData)[idx + 1]);
			rgba.b = (uint32_t)(((uint16_t*)m_ImageData)[idx + 2]);
			rgba.a = (uint32_t)(((uint16_t*)m_ImageData)[idx + 3]);
		}

		return rgba;
	}

	//============================================================================================================//
	//============================================================================================================//
	//============================================================================================================//

	DirectXTexture3D* DirectXTexture3D::LoadFromFiles(const std::vector<std::string>& filepaths)
	{
		if (filepaths.size() != 6)
		{
			IFR_LOG_ERROR("Texture3D>    [-] Failed to load cube map. " + std::to_string(filepaths.size()) + " out of 6 texture paths were provided [-]");
			return nullptr;
		}

		std::vector<unsigned char*> texture_data;

		int width = 0, height = 0, channels;
		for (auto& path : filepaths)
		{
			stbi_set_flip_vertically_on_load(0);

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			texture_data.push_back(data);

			if (!data)
			{
				for (auto& tex : texture_data)
					stbi_image_free(tex);

				IFR_LOG_ERROR("[-] Failed to load one of the textures in texture3D: " + path + " [-]");
				return nullptr;
			}
		}

		DirectXTexture3D* instance = new DirectXTexture3D(texture_data, width, height);
		instance->m_ImageData = texture_data;

		IFR_LOG_SUCCESS("Texture3D>    [+] Loaded 6 textures starting with " + filepaths[0] + " [+]");
		return instance;
	}

	DirectXTexture3D::DirectXTexture3D(std::vector<unsigned char*> texture_data, uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
		texture_desc.Width = m_Width;
		texture_desc.Height = m_Height;
		texture_desc.ArraySize = 6; // because six textures
		texture_desc.MipLevels = 1;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SRVDesc.TextureCube.MipLevels = 1;
		SRVDesc.TextureCube.MostDetailedMip = 0;

		D3D11_SUBRESOURCE_DATA pData[6];
		for (int i = 0; i < 6; i++)
		{
			// Pointer to the pixel data
			pData[i].pSysMem = texture_data[i];

			// Line width in bytes
			pData[i].SysMemPitch = 4 * height;
			
			// This is only used for 3d textures.
			pData[i].SysMemSlicePitch = 0;
		}

		ComPtr<ID3D11Texture2D> texture_buffer;
		HRESULT result = DirectXResources::s_Device->CreateTexture2D(&texture_desc, &pData[0], texture_buffer.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("Texture3D> Failed to create ID3D11Texture2D | HRESULT: " + std::to_string(result));
			return;
		}

		result = DirectXResources::s_Device->CreateShaderResourceView(texture_buffer.Get(),
			&SRVDesc, m_ShaderResource.GetAddressOf());

		if (FAILED(result))
			IFR_LOG_ERROR("Texture3D> Failed to create shader resource view");
	}

	DirectXTexture3D::~DirectXTexture3D()
	{
		for (auto& img : m_ImageData)
			stbi_image_free(img);
	}
	
	void* DirectXTexture3D::GetData() const
	{
		return m_ImageData[0];
	}
	
	void DirectXTexture3D::Bind(uint32_t slot) const
	{
		if (ShaderLocation == TextureShaderLocation::PIXEL_SHADER) DirectXResources::s_DeviceContext->PSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
		if (ShaderLocation == TextureShaderLocation::DOMAIN_SHADER) DirectXResources::s_DeviceContext->DSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
	}
}
