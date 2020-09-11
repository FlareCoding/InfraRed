#pragma once
#include <core/IFRCore.h>

namespace ifr
{
	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual ~RenderingContext() = default;
	};
}
