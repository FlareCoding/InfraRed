#pragma once
#include "../IComponent.h"
#include <model_data/StaticModel.h>

namespace ifr
{
	class StaticRenderComponent : public IComponent
	{
	public:
		StaticRenderComponent() = default;

		// Inherited via IComponent
		virtual ComponentType GetType() override;
		virtual void OnAttach() override {}
		virtual void OnUpdate() override {}

		StaticModel Model;
		bool Wireframe = false;
		bool ShouldRender = true;
	};
}
