#pragma once
#include "Texture.h"

namespace ifr
{
	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind() const = 0;
		virtual void RestoreMainBackBuffer() const = 0;

		virtual void ClearBufferColor(const glm::vec4& color) const = 0;

		virtual Ref<Texture> GetColorAttachmentTexture() const = 0;
	};
}
