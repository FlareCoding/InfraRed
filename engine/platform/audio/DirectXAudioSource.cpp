#include "DirectXAudioSource.h"
#include <platform/DirectXResources.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

namespace ifr
{
	HRESULT FindChunkWAV(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) return S_FALSE;
		}

		return S_OK;
	}

	HRESULT ReadChunkDataWAV(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	DirectXAudioSource* DirectXAudioSource::LoadFromFileWAV(const std::string& path)
	{
		DirectXAudioSource* instance = new DirectXAudioSource();

		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

		HANDLE hFile = CreateFile(
			std::wstring(path.begin(), path.end()).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		if (INVALID_HANDLE_VALUE == hFile)
			IFR_LOG_ERROR(std::to_string(GetLastError()));

		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			IFR_LOG_ERROR(std::to_string(GetLastError()));

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		// Check the file type, should be fourccWAVE or 'XWMA'
		FindChunkWAV(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		ReadChunkDataWAV(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)
			IFR_LOG_ERROR("Error reading file");

		FindChunkWAV(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkDataWAV(hFile, &wfx, dwChunkSize, dwChunkPosition);

		// Fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunkWAV(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkDataWAV(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		instance->m_ChunkSize = dwChunkSize;
		CloseHandle(hFile);

		HRESULT result = DirectXResources::s_XAudio2->CreateSourceVoice(&instance->m_SourceVoice, (WAVEFORMATEX*)&wfx);
		if (FAILED(result))
		{
			IFR_LOG_ERROR("Failed to create source voice");
			delete instance;
			delete[] pDataBuffer;
			return nullptr;
		}

		instance->m_DataBuffer = pDataBuffer;
		
		IFR_LOG_SUCCESS("AudioSource>  [+] Loaded " + path + " [+]");
		return instance;
	}

	void DirectXAudioSource::Start() const
	{
		m_SourceVoice->FlushSourceBuffers();

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = m_ChunkSize;
		buffer.pAudioData = m_DataBuffer;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		m_SourceVoice->SubmitSourceBuffer(&buffer);

		m_SourceVoice->Start(0);
	}
	
	void DirectXAudioSource::Stop() const
	{
		m_SourceVoice->Stop();
	}

	void DirectXAudioSource::Destroy() const
	{
		if (m_SourceVoice)
			m_SourceVoice->DestroyVoice();

		if (m_DataBuffer)
			delete[] m_DataBuffer;
	}

	void DirectXAudioSource::SetVolume(float volume) const
	{
		m_SourceVoice->SetVolume(volume / 100.0f);
	}
}
