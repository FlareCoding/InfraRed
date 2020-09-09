#include "DirectXVertexArray.h"
#include "DirectXVertexBuffer.h"
#include "DirectXIndexBuffer.h"

namespace ifr
{
	void DirectXVertexArray::Bind() const
	{
		UINT offset = 0;
		for (auto i = 0; i < m_VertexBuffers.size(); i++)
		{
			DirectXResources::s_DeviceContext->IASetVertexBuffers(
				i,
				1,
				static_cast<DirectXVertexBuffer*>(m_VertexBuffers.at(i).get())->GetAddressOf(),
				static_cast<DirectXVertexBuffer*>(m_VertexBuffers.at(i).get())->GetStridePtr(),
				&offset
			);
		}

		m_IndexBuffer->Bind();
	}

	void DirectXVertexArray::Unbind() const {}

	void DirectXVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
	{
		m_VertexBuffers.push_back(vertex_buffer);
	}

	void DirectXVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
	{
		m_IndexBuffer = index_buffer;
	}

	const std::vector<Ref<VertexBuffer>>& DirectXVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& DirectXVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}
