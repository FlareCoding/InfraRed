#pragma once
#include "../Components.h"
#include "../Entity.h"

namespace ifr
{
	class RenderComponentSystem
	{
	public:
		static void Init();
		static void Update(const glm::vec4& ClipPlane = glm::vec4(0, -1, 0, 100000));

	private:
		static void RenderStaticComponent(Entity entity, StaticRenderComponent& component, const glm::vec4& ClipPlane);
		static void RenderAnimatedComponent(Entity entity, AnimatedRenderComponent& component, const glm::vec4& ClipPlane);

		static Ref<Texture> s_DefaultTexture;
	};
}
