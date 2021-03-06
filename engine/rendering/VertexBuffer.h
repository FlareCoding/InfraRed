#pragma once
#include <core/IFRCore.h>
#include "VertexData.h"
#include "BufferLayout.h"
#include <vector>

namespace ifr
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void UpdateContents(std::vector<Vertex>& vertices) = 0;
		virtual void UpdateContents(void* data, size_t size) = 0;

		static Ref<VertexBuffer> Create(std::vector<Vertex>& vertices);
		static Ref<VertexBuffer> Create(void* data, size_t size, uint32_t stride);
	};
}
