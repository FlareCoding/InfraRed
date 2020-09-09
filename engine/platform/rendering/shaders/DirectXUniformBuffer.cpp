#include "DirectXUniformBuffer.h"

namespace ifr
{
	DirectXUniformBuffer::DirectXUniformBuffer(const std::string& name, const std::vector<UniformVariable>& layout, 
		UniformBufferParentShader parent, uint32_t slot)
		: UniformBuffer(name, layout, parent, slot)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(m_DataSize + (16 - (m_DataSize % 16)));
		desc.StructureByteStride = 0;

		DirectXResources::s_Device->CreateBuffer(&desc, 0, m_Buffer.GetAddressOf());
	}

	void DirectXUniformBuffer::UploadToShader()
	{
		D3D11_MAPPED_SUBRESOURCE mr;
		HRESULT result = DirectXResources::s_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		if (FAILED(result))
		{
			IFR_LOG_ERROR("Failed to map constant buffer");
			return;
		}

		CopyMemory(mr.pData, m_Data, m_DataSize);
		DirectXResources::s_DeviceContext->Unmap(m_Buffer.Get(), 0);

		switch (m_ParentShader)
		{
		case UniformBufferParentShader::VERTEX_SHADER:	{ DirectXResources::s_DeviceContext->VSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::HULL_SHADER:	{ DirectXResources::s_DeviceContext->HSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::DOMAIN_SHADER:	{ DirectXResources::s_DeviceContext->DSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::PIXEL_SHADER:	{ DirectXResources::s_DeviceContext->PSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break;  }
		}
	}
}
