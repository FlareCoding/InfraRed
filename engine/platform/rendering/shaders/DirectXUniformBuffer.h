#pragma once
#include "Rendering/Shaders/UniformBuffer.h"
#include "../../DirectXResources.h"
#include <vector>

namespace ifr
{
	class DirectXUniformBuffer : public UniformBuffer
	{
	public:
		DirectXUniformBuffer(const std::string& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32_t slot = 0);

		virtual void UploadToShader() override;

	private:
		ComPtr<ID3D11Buffer> m_Buffer;
	};
}
