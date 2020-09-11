#include "RenderComponentSystem.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderer3D.h"
#include "../ComponentManager.h"

namespace ifr
{
	Ref<Texture> RenderComponentSystem::s_DefaultTexture = nullptr;

	void RenderComponentSystem::Init()
	{
		s_DefaultTexture = Texture2D::CreateFromColor({ 120, 120, 120 });
	}

	void RenderComponentSystem::Update(const glm::vec4& ClipPlane)
	{
		Renderer3D::BindBasicShader();

		StaticRenderComponentManager.ForEachComponent([&ClipPlane](Entity entity, StaticRenderComponent& component) {
			if (!component.Enabled) return;

			if (component.ShouldRender)
			{
				if (component.Wireframe) Renderer::SetWireframe(true);

				RenderStaticComponent(entity, component, ClipPlane);

				if (component.Wireframe) Renderer::SetWireframe(false);
			}
		});

		bool bShouldRenderBoundingBoxes = false;

		ColliderComponentManager.ForEachComponent([&bShouldRenderBoundingBoxes](Entity entity, ColliderComponent& component) {
			if (!component.Enabled) return;

			if (component.ShouldRenderBoundingVolume && component.ObjectOrientedBox)
			{
				if (!bShouldRenderBoundingBoxes)
				{
					Renderer::SetWireframe(true);
					bShouldRenderBoundingBoxes = true;
				}

				auto rOBB = component.ObjectOrientedBox;
				auto rAABB = rOBB->GetAABB();

				rOBB->UpdateMesh();
				rAABB->UpdateMesh();

				Renderer3D::DrawMesh(rOBB->GetMesh(), rOBB->position, rOBB->rotation, nullptr, rOBB->DebugColor, 1.0f, rOBB->scale);
			}
		});

		if (bShouldRenderBoundingBoxes) Renderer::SetWireframe(false);

		Renderer3D::BindBasicAnimShader();

		AnimatedRenderComponentManager.ForEachComponent([&ClipPlane](Entity entity, AnimatedRenderComponent& component) {
			if (!component.Enabled) return;

			if (component.ShouldRender)
			{
				if (component.Wireframe) Renderer::SetWireframe(true);

				RenderAnimatedComponent(entity, component, ClipPlane);

				if (component.Wireframe) Renderer::SetWireframe(false);
			}
		});
	}

	void RenderComponentSystem::RenderStaticComponent(Entity entity, StaticRenderComponent& component, const glm::vec4& ClipPlane)
	{
		TransformComponent* transform = (TransformComponent*)GetComponent(entity, ComponentType::Transform);
		if (!transform) return;

		StaticModel& model = component.Model;

		for (size_t i = 0; i < model.GetMeshCount(); i++)
		{
			Renderer3D::DrawMesh(model.GetMesh(i), transform->Position, transform->Rotation, transform->Scale, model.Material, ClipPlane);
		}
	}

	void RenderComponentSystem::RenderAnimatedComponent(Entity entity, AnimatedRenderComponent& component, const glm::vec4& ClipPlane)
	{
		TransformComponent* transform	= (TransformComponent*)GetComponent(entity, ComponentType::Transform);
		if (!transform) return;

		auto timestep					= Time::GetTimestep();
		auto& model						= component.Model;
		auto& animation					= model.GetActiveAnimation();

		animation.Update(timestep);

		Ref<Texture> active_texture = s_DefaultTexture;

		Renderer3D::DrawAnimatedMesh(model.mesh, animation.GetCurrentPoseTransforms(), transform->Position, transform->Rotation, transform->Scale, model.material, ClipPlane);
	}
}
