#include "AnimationData.h"

namespace ifr
{
	glm::mat4 BoneTransform::GetLocalTransform()
	{
		glm::mat4 mat = glm::mat4(1.0f);

		mat = glm::translate(mat, Translation);
		mat = mat * glm::toMat4(Rotation);

		return mat;
	}

	BoneTransform BoneTransform::Interpolate(BoneTransform first, BoneTransform second, float progression)
	{
		float x = first.Translation.x + (second.Translation.x - first.Translation.x) * progression;
		float y = first.Translation.y + (second.Translation.y - first.Translation.y) * progression;
		float z = first.Translation.z + (second.Translation.z - first.Translation.z) * progression;

		glm::vec3 pos = { x, y, z };
		glm::quat rot = Interpolate(first.Rotation, second.Rotation, progression);

		BoneTransform result;
		result.Translation = pos;
		result.Rotation = rot;

		return result;
	}

	glm::quat BoneTransform::Interpolate(glm::quat a, glm::quat b, float blend)
	{
		glm::quat result = glm::quat(1, 0, 0, 0);

		float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		float blendI = 1.0f - blend;

		if (dot < 0)
		{
			result.w = blendI * a.w + blend * -b.w;
			result.x = blendI * a.x + blend * -b.x;
			result.y = blendI * a.y + blend * -b.y;
			result.z = blendI * a.z + blend * -b.z;
		}
		else
		{
			result.w = blendI * a.w + blend * b.w;
			result.x = blendI * a.x + blend * b.x;
			result.y = blendI * a.y + blend * b.y;
			result.z = blendI * a.z + blend * b.z;
		}

		result = glm::normalize(result);
		return result;
	}
}
