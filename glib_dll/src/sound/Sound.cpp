#include "glib/sound/Sound.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

namespace glib
{
	class SoundImpl
	{
	private:
		ALuint m_Source;
		AudioDataSource* m_DataSource;
	public:
		SoundImpl(AudioDataSource* source) : m_DataSource(source)
		{
			alGenSources(1, &m_Source);
			alSourcei(m_Source, AL_BUFFER, source->GetID());
			alSourcef(m_Source, AL_GAIN, 0.025f);
		}

		~SoundImpl()
		{
			Stop();
			alDeleteSources(1, &m_Source);
		}

		void Play()
		{
			alSourcePlay(m_Source);
		}

		void Stop()
		{
			alSourceStop(m_Source);
		}

		void Pause()
		{
			alSourcePause(m_Source);
		}

		void Resume()
		{
			alSourcePlay(m_Source);
		}

		float GetTimePosition()
		{
			int offset;
			alGetSourcei(m_Source, AL_SAMPLE_OFFSET, &offset);
			float pos = (float)offset / (float)m_DataSource->GetSampleRate();
			return pos * 1000.0f;
		}
	};
}

using namespace glib;

glib::Sound::Sound(AudioDataSource* source)
{
	impl = new SoundImpl(source);
}

glib::Sound::~Sound()
{
	delete impl;
}

void glib::Sound::Play()
{
	impl->Play();
}

void glib::Sound::Stop()
{
	impl->Stop();
}

void glib::Sound::Pause()
{
	impl->Pause();
}

void glib::Sound::Resume()
{
	impl->Resume();
}

float glib::Sound::GetTimePosition()
{
	return impl->GetTimePosition();
}
