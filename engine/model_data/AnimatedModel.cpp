#include "AnimatedModel.h"

namespace ifr
{
	AnimatedModel::AnimatedModel()
	{
	}

	AnimatedModel::AnimatedModel(const Ref<Mesh>& skin, const std::vector<Animation>& animations) : mesh(skin), animations(animations)
	{
	}
	
	void AnimatedModel::SetActiveAnimation(const std::string& name)
	{
		for (size_t i = 0; i < animations.size(); i++)
		{
			if (animations[i].Name._Equal(name))
			{
				if (animations[m_ActiveAnimationIndex].IsPlaying())
					animations[m_ActiveAnimationIndex].Stop();

				m_ActiveAnimationIndex = i;
				break;
			}
		}
	}

	Animation* AnimatedModel::GetAnimation(const std::string& name)
	{
		for (auto& animation : animations)
			if (animation.Name._Equal(name))
				return &animation;

		return nullptr;
	}
	
	Animation& AnimatedModel::GetActiveAnimation()
	{
		return animations[m_ActiveAnimationIndex];
	}
}
