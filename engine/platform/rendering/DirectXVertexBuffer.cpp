#include "DirectXVertexBuffer.h"

namespace ifr
{
	DirectXVertexBuffer::DirectXVertexBuffer(std::vector<Vertex>& vertices)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = vertices.size();
		if (m_Stride.get() == nullptr)
			m_Stride = std::make_unique<UINT>(sizeof(Vertex));

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = sizeof(Vertex) * vertices.size();
		vbedsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbedsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &vertices[0];

		DirectXResources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	DirectXVertexBuffer::DirectXVertexBuffer(void* data, size_t size, uint32_t stride)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = size;
		if (m_Stride.get() == nullptr)
			m_Stride = std::make_unique<UINT>(stride);

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = size;
		vbedsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbedsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = data;

		DirectXResources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	void DirectXVertexBuffer::Bind() {}

	void DirectXVertexBuffer::Unbind() {}
	
	void DirectXVertexBuffer::UpdateContents(std::vector<Vertex>& vertices)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		DirectXResources::s_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &vertices[0], sizeof(Vertex) * vertices.size());
		DirectXResources::s_DeviceContext->Unmap(m_Buffer.Get(), 0);
	}

	void DirectXVertexBuffer::UpdateContents(void* data, size_t size)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		DirectXResources::s_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, data, size);
		DirectXResources::s_DeviceContext->Unmap(m_Buffer.Get(), 0);
	}
}
