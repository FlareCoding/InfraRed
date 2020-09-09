#pragma once
#include <model_data/StaticModel.h>
#include <model_data/AnimatedModel.h>

namespace ifr
{
	class AssimpLoader
	{
	public:
		static StaticModel Load(const std::string& filepath, bool bShouldApplyCorrectionMatrix = false);
		static AnimatedModel LoadAnimated(const std::string& filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}
