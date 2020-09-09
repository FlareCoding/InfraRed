#pragma once
#include <rendering/IndexBuffer.h>
#include <platform/DirectXResources.h>
#include <vector>

namespace ifr
{
	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(std::vector<int>& indices);

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual int GetCount() override;

	private:
		ComPtr<ID3D11Buffer>	m_Buffer;
		UINT					m_BufferSize;
	};
}
