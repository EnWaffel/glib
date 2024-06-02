#include "glib/sound/SoundManager.h"

#include "glib/utils/AudioFileReader.h"
#include "glib/glibError.h"

#include <vector>
#include <map>

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

extern int __GLIB_ERROR_CODE;
extern void glib_print_error();

namespace glib
{
	class SoundManagerImpl
	{
	private:
		// np = non persistent (aka the sounds are deleted after they played)
		std::vector<Sound*> m_npSounds;
		// p = persistent
		std::vector<Sound*> m_pSounds;
		std::map<std::string, AudioDataSource*> m_Sources;
		ALCdevice* m_Device;
		ALCcontext* m_Context;
	public:
		SoundManagerImpl() : m_Device(nullptr), m_Context(nullptr)
		{
		}

		~SoundManagerImpl()
		{
			for (Sound* snd : m_npSounds)
			{
				delete snd;
			}
			for (Sound* snd : m_pSounds)
			{
				delete snd;
			}
			for (const auto& v : m_Sources)
			{
				delete v.second;
			}

			alcMakeContextCurrent(m_Context);
			alcCloseDevice(m_Device);
			alcDestroyContext(m_Context);
		}

		AudioDataSource* GetDataSource(const std::string& name) const
		{
			if (m_Sources.count(name) < 1) return nullptr;
			return m_Sources.at(name);
		}

		Sound* CreateSound(AudioDataSource* source)
		{
			Sound* snd = _CreateSnd(source);
			m_npSounds.push_back(snd);
			return snd;
		}

		Sound* CreatePersistantSound(AudioDataSource* source)
		{
			Sound* snd = _CreateSnd(source);
			m_pSounds.push_back(snd);
			return snd;
		}

		AudioDataSource* CreateSourceFromFile(const std::string& name, const std::string& path)
		{
			AudioData data = AudioFileReader::ReadFile(path);
			if (data.buf == nullptr)
			{
				if (data.depth == 1000)
				{
					__GLIB_ERROR_CODE = GLIB_SOUND_UNSUPPORTED_FORMAT;
					glib_print_error();
				}
				else if (data.depth == 3000)
				{
					__GLIB_ERROR_CODE = GLIB_SOUND_FILE_OPEN_FAIL;
					glib_print_error();
				}
				return nullptr;
			}

			ALuint buf;
			alGenBuffers(1, &buf);

			ALenum format = AL_INVALID_ENUM;
			if (data.depth == 8)
			{
				if (data.channels == 1)
				{
					format = AL_FORMAT_MONO8;
				}
				else if (data.channels == 2)
				{
					format = AL_FORMAT_STEREO8;
				}
			}
			else if (data.depth == 16)
			{
				if (data.channels == 1)
				{
					format = AL_FORMAT_MONO16;
				}
				else if (data.channels == 2)
				{
					format = AL_FORMAT_STEREO16;
				}
			}

			alBufferData(buf, format, data.buf, data.size, data.sampleRate);
			delete[] data.buf;

			AudioDataSource* source = new AudioDataSource(buf, data.sampleRate);
			m_Sources.insert({ name, source });

			return source;
		}

		void ChangeOutputDevice(const std::string& device)
		{
			if (m_Context != nullptr && m_Device != nullptr)
			{
				alcMakeContextCurrent(m_Context);
				alcCloseDevice(m_Device);
				alcDestroyContext(m_Context);
			}

			if (device.empty())
			{
				m_Device = alcOpenDevice(nullptr);
			}
			else
			{
				m_Device = alcOpenDevice(device.c_str());
			}
			
			m_Context = alcCreateContext(m_Device, nullptr);
			alcMakeContextCurrent(m_Context);
		}

		void Update()
		{
			for (Sound* snd : m_npSounds)
			{

			}
		}
	private:
		Sound* _CreateSnd(AudioDataSource* source)
		{
			if (source == nullptr) return nullptr;
			Sound* snd = new Sound(source);
			return snd;
		}
	};
}

using namespace glib;

glib::SoundManager::SoundManager()
{
	impl = new SoundManagerImpl;
}

glib::SoundManager::~SoundManager()
{
	delete impl;
}

Sound* glib::SoundManager::CreateSound(AudioDataSource* source)
{
	return impl->CreateSound(source);
}

Sound* glib::SoundManager::CreateSound(const std::string& sourceName)
{
	return impl->CreateSound(impl->GetDataSource(sourceName));
}

Sound* glib::SoundManager::CreatePersistantSound(AudioDataSource* source)
{
	return impl->CreatePersistantSound(source);
}

Sound* glib::SoundManager::CreatePersistantSound(const std::string& sourceName)
{
	return impl->CreatePersistantSound(impl->GetDataSource(sourceName));
}

AudioDataSource* glib::SoundManager::CreateSourceFromFile(const std::string& name, const std::string& path)
{
	return impl->CreateSourceFromFile(name, path);
}

void glib::SoundManager::ChangeOutputDevice(const std::string& device)
{
	impl->ChangeOutputDevice(device);
}

void glib::SoundManager::Update()
{
	impl->Update();
}
