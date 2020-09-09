#include "Animation.h"

namespace ifr
{	
	Animation::Animation(float duration, float ticks_per_second, glm::mat4 inverse_transform, int bone_count, Bone root_bone, glm::mat4 correction_matrix)
		: Duration(duration), TicksPerSecond(ticks_per_second), m_InverseTransform(inverse_transform), m_RootBone(root_bone), 
			m_BoneCount((uint32_t)bone_count), m_CorrectionMatrix(correction_matrix)
	{
		for (size_t i = 0; i < bone_count; i++)
			m_BoneFrameTransforms.push_back(glm::mat4(1.0f));
	}

	void Animation::CalculateFinalBoneTransforms(Bone& bone, glm::mat4 parent_transform, float animation_time)
	{
		auto frames = GetPreviousAndNextFrames(bone, animation_time);
		auto progression = CalculateProgression(bone, frames.first, frames.second, animation_time);

		// Sanity check
		if (frames.first > bone.Keyframes.size() - 1)
			frames.first = 0;

		if (frames.second > bone.Keyframes.size() - 1)
			frames.second = 0;

		glm::mat4 current_pose = InterpolatePoses(bone.Keyframes[frames.first].Transform, bone.Keyframes[frames.second].Transform, progression);

		glm::mat4 transform = parent_transform * current_pose;

		bone.FinalTransform = m_InverseTransform * transform * bone.OffsetMatrix;

		for (auto& child : bone.Children)
			CalculateFinalBoneTransforms(child, transform, animation_time);
	}

	void Animation::AddBoneTransform(Bone& bone)
	{
		m_BoneFrameTransforms[bone.ID] = bone.UserTransformation * m_CorrectionMatrix * bone.FinalTransform;

		for (auto& child : bone.Children)
			AddBoneTransform(child);
	}

	std::vector<glm::mat4>& Animation::GetCurrentPoseTransforms()
	{
		if (m_IsPlaying)
		{
			CalculateFinalBoneTransforms(m_RootBone, glm::mat4(1.0f), CurrentAnimationTime);
			AddBoneTransform(m_RootBone);
		}
		else
		{
			for (size_t i = 0; i < m_BoneCount; i++)
				m_BoneFrameTransforms[i] = m_CorrectionMatrix;
		}

		return m_BoneFrameTransforms;
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		Bone* result = nullptr;

		ForEachBone(m_RootBone, [&name, &result](Bone& bone) {
			if (bone.Name._Equal(name))
				result = &bone;
		});

		return result;
	}

	void Animation::ForEachBone(Bone& bone, std::function<void(Bone&)> lambda)
	{
		lambda(bone);

		for (auto& child : bone.Children)
			ForEachBone(child, lambda);
	}

	std::pair<size_t, size_t> Animation::GetPreviousAndNextFrames(Bone& bone, float animation_time)
	{
		float PreviousFrame = bone.Keyframes[0].Timestamp;
		float NextFrame = bone.Keyframes[0].Timestamp;

		size_t PreviousFrameIndex = 0;
		size_t NextFrameIndex = 0;

		for (size_t i = 1; i < bone.Keyframes.size(); i++)
		{
			NextFrame = bone.Keyframes[i].Timestamp;
			NextFrameIndex = i;

			if (NextFrame > animation_time)
				break;

			PreviousFrame = bone.Keyframes[i].Timestamp;
			PreviousFrameIndex = i;
		}

		return { PreviousFrameIndex, NextFrameIndex };
	}

	float Animation::CalculateProgression(Bone& bone, size_t previous_frame_index, size_t next_frame_index, float animation_time)
	{
		float TotalTime = bone.Keyframes[next_frame_index].Timestamp - bone.Keyframes[previous_frame_index].Timestamp;
		float CurrentTime = animation_time - bone.Keyframes[previous_frame_index].Timestamp;
		return CurrentTime / TotalTime;
	}

	glm::mat4 Animation::InterpolatePoses(BoneTransform previous_pose, BoneTransform next_pose, float progression)
	{
		return BoneTransform::Interpolate(previous_pose, next_pose, progression).GetLocalTransform();
	}

	void Animation::Play()
	{
		m_IsPlaying = true;
	}

	void Animation::Pause()
	{
		m_IsPlaying = false;
	}

	void Animation::Stop()
	{
		m_IsPlaying = false;
		CurrentAnimationTime = 0;
	}

	void Animation::Update(Timestep ts)
	{
		if (!m_IsPlaying) return;

		if (CurrentAnimationTime > Duration)
		{
			if (ShouldLoopAnimation)
			{
				CurrentAnimationTime = 0;
			}
			else
			{
				Stop();
				return;
			}
		}

		CurrentAnimationTime += ((float)ts * AnimationSpeed) / AnimationSpeedDenominator;
	}
}
