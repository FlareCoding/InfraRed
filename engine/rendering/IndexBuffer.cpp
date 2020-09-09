#include "IndexBuffer.h"
#include "Renderer.h"
#include <platform/Rendering/DirectXIndexBuffer.h>

namespace ifr
{
	Ref<IndexBuffer> IndexBuffer::Create(std::vector<int>& indices)
	{
		return Ref<IndexBuffer>(new DirectXIndexBuffer(indices));
	}
}
