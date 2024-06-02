#pragma once

#include "../DLLDefs.h"
#include "AudioDataSource.h"

namespace glib
{
	class SoundImpl;

	class Sound
	{
	private:
		SoundImpl* impl;
	public:
		Sound(AudioDataSource* source);
		~Sound();

		GLIB_API void Play();
		GLIB_API void Stop();
		GLIB_API void Pause();
		GLIB_API void Resume();
		GLIB_API float GetTimePosition();
	};
}