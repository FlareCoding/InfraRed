#pragma once
#include <rendering/shaders/Shaders.h>
#include <model_data/StaticModel.h>
#include <camera/ICamera.h>
#include <rendering/FrameBuffer.h>

namespace ifr
{
	class WaterChunk
	{
	public:
		static Ref<WaterChunk> Create();

		void BindReflectionBuffer();
		void BindRefractionBuffer();
		void RestoreMainBackBuffer(uint32_t offsetx, uint32_t offsety, uint32_t width, uint32_t height);

		void Render(ICamera& camera, const Light& light);

		Ref<Texture> DistortionMap = nullptr;
		Ref<Texture> NormalMap = nullptr;

		float Tiling = 4.0f;
		glm::vec3 Position = glm::vec3(0, 0, 0);
		float Rotation = 0;
		float Size = 5.0f;

		float WaveStrength = 30.0f;
		float WaveSpeed = 10.0f;

	private:
		WaterChunk();

		static const uint32_t s_ReflectionWidth  = 320;
		static const uint32_t s_ReflectionHeight = 180;

		static const uint32_t s_RefractionWidth  = 1280;
		static const uint32_t s_RefractionHeight = 720;

		Ref<FrameBuffer> m_ReflectionBuffer = nullptr;
		Ref<FrameBuffer> m_RefractionBuffer = nullptr;

	private:
		Ref<Mesh> m_WaterPlane = nullptr;
		Ref<WaterShader> m_Shader = nullptr;

	private:
		float m_MoveFactor = 0.0f;
	};
}
