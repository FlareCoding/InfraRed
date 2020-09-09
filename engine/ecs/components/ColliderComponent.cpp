#include "ColliderComponent.h"
#include <ecs/Entity.h>

#include "StaticRenderComponent.h"
#include "AnimatedRenderComponent.h"

namespace ifr
{
	ComponentType ColliderComponent::GetType()
	{
		return ComponentType::Collider;
	}
	
	void ColliderComponent::OnAttach()
	{
		m_TransformComponentLink = (TransformComponent*)GetComponent(Owner, ComponentType::Transform);

		// trying to get model's bounding box data by looking at 
		// either static or animated render components if they exist
		auto* static_render_component = (StaticRenderComponent*)GetComponent(Owner, ComponentType::StaticRender);
		if (static_render_component)
		{
			auto& box_data = static_render_component->Model.DefaultBoundingBoxData;
			ObjectOrientedBox = Ref<OBB>(new OBB(box_data.xMax, box_data.xMin, box_data.yMax, box_data.yMin, box_data.zMax, box_data.zMin));
		}
		else
		{
			// if static render component doesn't exist, check for animated
			auto* animated_render_component = (AnimatedRenderComponent*)GetComponent(Owner, ComponentType::AnimatedRender);
			if (animated_render_component)
			{
				auto& box_data = animated_render_component->Model.DefaultBoundingBoxData;
				ObjectOrientedBox = Ref<OBB>(new OBB(box_data.xMax, box_data.xMin, box_data.yMax, box_data.yMin, box_data.zMax, box_data.zMin, animated_render_component->Model.animation.GetCorrectionMatrix()));
			}
		}
	}
	
	void ColliderComponent::OnUpdate()
	{
		if (ObjectOrientedBox)
		{
			ObjectOrientedBox->SetTransform(m_TransformComponentLink->Position, m_TransformComponentLink->Rotation, m_TransformComponentLink->Scale);
			ObjectOrientedBox->UpdateAABB();
		}
	}
}
