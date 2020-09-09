#include "VertexArray.h"
#include "Renderer.h"
#include <platform/rendering/DirectXVertexArray.h>

namespace ifr
{
	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<VertexArray>(new DirectXVertexArray());
	}
}
