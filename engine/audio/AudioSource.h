#pragma once
#include <core/IFRCore.h>

namespace ifr
{
	class AudioSource
	{
	public:
		static Ref<AudioSource> LoadFromFileWAV(const std::string& path);

		virtual void Start() const = 0;
		virtual void Stop() const = 0;
		virtual void Destroy() const = 0;
		virtual void SetVolume(float volume) const = 0;

		std::string Name = "AudioSource";
	};
}
