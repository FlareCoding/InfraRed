#include "FrameBuffer.h"
#include <platform/rendering/DirectXFrameBuffer.h>

namespace ifr
{
	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		return Ref<FrameBuffer>(new DirectXFrameBuffer(width, height));
	}
}
