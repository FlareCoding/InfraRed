#pragma once
#include <model_data/StaticModel.h>
#include <rendering/shaders/Shaders.h>
#include <vector>

namespace ifr
{
	class GrassPatch
	{
	public:
		static Ref<GrassPatch> Create();

		glm::vec3 Position = glm::vec3(0, 0, 0);
		float Scale = 1.0f;

		Ref<Texture> texture = nullptr;

		void AddCluster(const glm::vec3& position);

		void Render(ICamera& camera, const Light& light);

	private:
		GrassPatch();

	private:
		struct GrassVertex
		{
			glm::vec3 position;
			glm::vec2 uv;
		};

		struct GrassInstance
		{
			glm::vec3	position;
			float		height;
			float		rotation;
		};

	private:
		Ref<VertexBuffer> m_InstanceBuffer;
		Ref<VertexArray>  m_VertexArray;
		Ref<GrassShader>  m_Shader;

		uint32_t					m_InstanceCount = 10000;
		std::vector<GrassInstance>	m_Instances;

	private:
		int		m_WindDirection = 1;
		float	m_WindRotation = 0.0f;
	};
}
