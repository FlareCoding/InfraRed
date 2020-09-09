#pragma once
#include <core/ifrcore.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ifr
{
	class BoneTransform
	{
	public:
		glm::vec3 Translation;
		glm::quat Rotation;

		glm::mat4 GetLocalTransform();

		static BoneTransform Interpolate(BoneTransform first, BoneTransform second, float progression);

	private:
		static glm::quat Interpolate(glm::quat a, glm::quat b, float blend);
	};

	struct Keyframe
	{
		float Timestamp;
		BoneTransform Transform;
	};
}
