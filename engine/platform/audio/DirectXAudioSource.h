#pragma once
#include <audio/AudioSource.h>

struct IXAudio2SourceVoice;

namespace ifr
{
	class DirectXAudioSource : public AudioSource
	{
	public:
		static DirectXAudioSource* LoadFromFileWAV(const std::string& path);

		virtual void Start() const override;
		virtual void Stop() const override;
		virtual void Destroy() const override;
		virtual void SetVolume(float volume) const override;

	private:
		IXAudio2SourceVoice*	m_SourceVoice = nullptr;
		unsigned long			m_ChunkSize = 0;
		unsigned char*			m_DataBuffer = nullptr;
	};
}
