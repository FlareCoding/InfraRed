#include "DirectXIndexBuffer.h"

namespace ifr
{
	DirectXIndexBuffer::DirectXIndexBuffer(std::vector<int>& indices)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = indices.size();

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int) * indices.size();
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &indices[0];

		DirectXResources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	void DirectXIndexBuffer::Bind()
	{
		DirectXResources::s_DeviceContext->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectXIndexBuffer::Unbind() {}

	int DirectXIndexBuffer::GetCount()
	{
		return (int)m_BufferSize;
	}
}
