#pragma once
#include <model_data/StaticModel.h>
#include <camera/ICamera.h>
#include <rendering/shaders/Shaders.h>

namespace ifr
{
	class Skybox
	{
	public:
		static Ref<Skybox> Create();

		Ref<Texture> texture = nullptr;

		void Render(ICamera& camera);

	private:
		Skybox();

	private:
		Ref<SkyboxShader> m_Shader;
		Ref<Mesh> m_Mesh;
	};
}
