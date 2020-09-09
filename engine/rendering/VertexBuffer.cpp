#include "VertexBuffer.h"
#include "Renderer.h"
#include <platform/rendering/DirectXVertexBuffer.h>

namespace ifr
{
	Ref<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices)
	{
		return Ref<VertexBuffer>(new DirectXVertexBuffer(vertices));
	}
	
	Ref<VertexBuffer> VertexBuffer::Create(void* data, size_t size, uint32_t stride)
	{
		return Ref<VertexBuffer>(new DirectXVertexBuffer(data, size, stride));
	}
}
