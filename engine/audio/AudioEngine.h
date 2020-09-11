#pragma once
#include "AudioSource.h"
#include <map>

namespace ifr
{
	class AudioEngine
	{
		friend class IFRApplication;

	public:
		static AudioEngine& Get();

		void RegisterAudioSource(Ref<AudioSource> source, const std::string& name);
		Ref<AudioSource> GetAudioSource(const std::string& name);

		Ref<AudioSource> LoadAndRegisterWAVAudioSource(const std::string& path, const std::string& name);

	private:
		static AudioEngine* s_Instance;
		std::map<std::string, Ref<AudioSource>> m_SourceMap;

	private:
		AudioEngine();
		void Shutdown();
	};
}
