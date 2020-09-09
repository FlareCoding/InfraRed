#include "AnimatedModel.h"

namespace ifr
{
	AnimatedModel::AnimatedModel()
	{
	}

	AnimatedModel::AnimatedModel(const Ref<Mesh>& skin, const Bone& root_bone, Animation animation) : mesh(skin), animation(animation)
	{
	}
}
