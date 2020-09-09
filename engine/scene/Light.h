#pragma once
#include <math/IFRMath.h>

namespace ifr
{
	struct Light
	{	
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		glm::vec3 position = glm::vec3(0, 1000, 0);
		glm::vec3 direction = glm::vec3(0, -1, 0);
		float intensity = 1.0f;
	};
}
