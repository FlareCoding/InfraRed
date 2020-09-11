#pragma once
#include "StaticModel.h"
#include "Animation.h"

namespace ifr
{
	class AnimatedModel
	{
	public:
		AnimatedModel();
		AnimatedModel(const Ref<Mesh>& skin, const std::vector<Animation>& animations);

		std::string Name = "";

		Ref<Mesh> mesh;
		Ref<MaterialInstance> material = nullptr;
		glm::vec4 Color = glm::vec4(1, 1, 1, 1);

		BoundingBoxData DefaultBoundingBoxData = BoundingBoxData();
		Ref<ConvexHull> DefaultConvexHull = nullptr;

		std::vector<Animation> animations;
		void SetActiveAnimation(const std::string& name);
		Animation* GetAnimation(const std::string& name);
		Animation& GetActiveAnimation();

	private:
		size_t m_ActiveAnimationIndex = 0;
	};
}
