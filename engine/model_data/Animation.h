#pragma once
#include <core/ifrcore.h>
#include "AnimationData.h"
#include "ModelData.h"
#include "Time/Time.h"

namespace ifr
{
	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& name, float duration, float ticks_per_second, glm::mat4 inverse_transform, int bone_count, Bone root_bone, glm::mat4 correction_matrix);

		std::string Name;

		float Duration = 0;
		float TicksPerSecond = 0;

		float AnimationSpeed = 1;
		float CurrentAnimationTime = 0;
		float AnimationSpeedDenominator = 1;

		bool ShouldLoopAnimation = true;

		bool IsPlaying() const { return m_IsPlaying; }

		void Play();
		void Pause();
		void Stop();
		void Update(Timestep ts);

		std::vector<glm::mat4>& GetCurrentPoseTransforms();

		glm::mat4 GetCorrectionMatrix() const { return m_CorrectionMatrix; }

		Bone* FindBone(const std::string& name);
		void ForEachBone(Bone& bone, std::function<void(Bone&)> lambda);

		Bone& GetRootBone() { return m_RootBone; }

	private:
		bool m_IsPlaying = false;

	private:
		glm::mat4 m_InverseTransform = glm::mat4(1.0f);
		Bone m_RootBone = Bone();
		uint32_t m_BoneCount = 0;
		glm::mat4 m_CorrectionMatrix = glm::mat4(1.0f);

	private:
		std::vector<glm::mat4> m_BoneFrameTransforms;

	private:
		void CalculateFinalBoneTransforms(Bone& bone, glm::mat4 parent_transform, float animation_time);
		void AddBoneTransform(Bone& bone);

		std::pair<size_t, size_t> GetPreviousAndNextFrames(Bone& bone, float animation_time);
		float CalculateProgression(Bone& bone, size_t previous_frame_index, size_t next_frame_index, float animation_time);
		glm::mat4 InterpolatePoses(BoneTransform previous_pose, BoneTransform next_pose, float progression);
	};
}
