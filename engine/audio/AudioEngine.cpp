#include "AudioEngine.h"

namespace ifr
{
	AudioEngine* AudioEngine::s_Instance = nullptr;

	AudioEngine::AudioEngine()
	{
		s_Instance = this;
	}

	AudioEngine& AudioEngine::Get()
	{
		return *s_Instance;
	}
	
	void AudioEngine::RegisterAudioSource(Ref<AudioSource> source, const std::string& name)
	{
		if (m_SourceMap.find(name) == m_SourceMap.end())
		{
			source->Name = name;
			m_SourceMap[name] = source;
		}
	}

	Ref<AudioSource> AudioEngine::GetAudioSource(const std::string& name)
	{
		if (m_SourceMap.find(name) != m_SourceMap.end())
			return m_SourceMap[name];
		else
			return nullptr;
	}

	Ref<AudioSource> AudioEngine::LoadAndRegisterWAVAudioSource(const std::string& path, const std::string& name)
	{
		Ref<AudioSource> source = AudioSource::LoadFromFileWAV(path);
		source->Name = name;
		RegisterAudioSource(source, name);
		return source;
	}

	void AudioEngine::Shutdown()
	{
		for (const auto& [name, source] : m_SourceMap)
		{
			source->Destroy();
		}
	}
}
