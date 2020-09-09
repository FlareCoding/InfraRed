#pragma once
#include <core/IFRCore.h>

namespace ifr
{
	enum class IFR_UB_SLOT
	{
		MVPMatrices = 0,
		TerrainBuffer = 1,
		MaterialBuffer = 2,
		VSSceneBuffer = 3,
		PSSceneBuffer = 4,
		BasicShaderBuffer = 5,
		WaterBuffer = 5,
		BoneTransforms = 8,
		SkyboxBuffer = 9,
	};
}
