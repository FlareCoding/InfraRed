#pragma once
#include "../IComponent.h"
#include <model_data/AnimatedModel.h>

namespace ifr
{
	class AnimatedRenderComponent : public IComponent
	{
	public:
		AnimatedRenderComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() override {}
		virtual void OnUpdate() override {}

		AnimatedModel Model;
		bool Wireframe = false;
		bool ShouldRender = true;
	};
}
