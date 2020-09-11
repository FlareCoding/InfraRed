#include "AudioSource.h"
#include <platform/audio/DirectXAudioSource.h>

namespace ifr
{
	Ref<AudioSource> AudioSource::LoadFromFileWAV(const std::string& path)
	{
		return Ref<AudioSource>(DirectXAudioSource::LoadFromFileWAV(path));
	}
}
