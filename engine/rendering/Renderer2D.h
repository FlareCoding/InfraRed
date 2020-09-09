#pragma once
#include <math/IFRMath.h>
#include <rendering/shaders/Shaders.h>
#include <camera/ICamera.h>
#include <rendering/Texture.h>
#include <model_data/shapes/Shapes.h>

namespace ifr
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(ICamera& camera);
		static void EndScene() {}

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float opacity = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& color, float opacity = 1.0f);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float opacity = 1.0f, float tiling = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float opacity = 1.0f, float tiling = 1.0f);

	private:
		struct SceneData
		{
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewMatrix;
		};

		static SceneData m_SceneData;

	private:
		static Ref<Mesh> m_Quad;
		static Ref<Mesh> m_Triangle;
		static Ref<BasicGUIShader> m_Shader;
		static Ref<Texture> m_WhiteTexture;
	};
}
