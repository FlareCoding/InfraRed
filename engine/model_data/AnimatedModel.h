#pragma once
#include "StaticModel.h"
#include "Animation.h"

namespace ifr
{
	class AnimatedModel
	{
	public:
		AnimatedModel();
		AnimatedModel(const Ref<Mesh>& skin, const Bone& root_bone, Animation animation);

		std::string Name = "";

		Ref<Mesh> mesh;
		Ref<MaterialInstance> material = nullptr;
		glm::vec4 Color = glm::vec4(1, 1, 1, 1);

		BoundingBoxData DefaultBoundingBoxData = BoundingBoxData();
		Ref<ConvexHull> DefaultConvexHull = nullptr;

		Animation animation;
	};
}
