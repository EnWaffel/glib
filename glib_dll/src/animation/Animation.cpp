#include "glib/animation/Animation.h"

#include <iostream>

namespace glib
{
	class AnimationImpl
	{
	public:
		std::vector<AnimationFrame> m_Frames;
		int m_FPS;
		bool m_Loop;
		bool m_Playing;
		int m_FrameIdx;
		float m_Waited;
		float m_ToWait;
		Animatable* m_Obj;
		bool m_Finished;
		bool m_FreeTextures = true;
		Vec2 m_Offset;
	public:
		AnimationImpl(std::vector<AnimationFrame> frames, int fps, bool loop) : m_Frames(frames), m_FPS(fps), m_Loop(loop), m_Playing(false), m_FrameIdx(0), m_Waited(0), m_Obj(nullptr), m_Finished(false), m_Offset(Vec2(0.0f, 0.0f))
		{
			m_ToWait = 1000.0f / fps;
		}

		~AnimationImpl()
		{
			if (!m_FreeTextures) return;
			for (const AnimationFrame& frame : m_Frames)
			{
				delete frame.tex;
			}
		}

		void Play(Animatable* obj)
		{
			m_Obj = obj;
			m_FrameIdx = 0;
			m_Waited = 0.0f;
			m_Playing = true;
			m_Finished = false;
			AnimationFrame& frame = m_Frames[m_FrameIdx];
			m_Obj->SetValues({ frame.tex, frame.x + m_Offset.x, frame.y + m_Offset.y, frame.w, frame.h });
			m_FrameIdx++;
		}

		void Pause()
		{
			m_Playing = false;
		}

		void Resume()
		{
			m_Playing = true;
		}

		void Stop()
		{
			m_Playing = false;
			m_Finished = true;
		}

		void Update(float delta)
		{
			if (!m_Playing) return;
			m_Waited += delta;
			if (m_Waited >= m_ToWait)
			{
				m_Waited = 0.0f;
				if (m_FrameIdx >= m_Frames.size())
				{
					if (m_Loop)
					{
						m_FrameIdx = 0;
						return;
					}
					m_Playing = false;
					m_Finished = true;
					return;
				}

				AnimationFrame& frame = m_Frames[m_FrameIdx];
				m_Obj->SetValues({ frame.tex, frame.x + m_Offset.x, frame.y + m_Offset.y, frame.w, frame.h });

				m_FrameIdx++;
			}
		}

		bool IsFinished()
		{
			return m_Finished;
		}

		void SetFPS(int fps)
		{
			m_FPS = fps;
		}

		void SetLooping(bool looping)
		{
			m_Loop = looping;
		}

		void SetOffset(const Vec2& offset)
		{
			m_Offset = offset;
		}
	};
}

using namespace glib;

glib::Animation::Animation(std::vector<AnimationFrame> frames, int fps, bool loop)
{
	impl = new AnimationImpl(frames, fps, loop);
}

glib::Animation::~Animation()
{
	delete impl;
}

void glib::Animation::Play(Animatable* obj) const
{
	impl->Play(obj);
}

void glib::Animation::Pause() const
{
	impl->Pause();
}

void glib::Animation::Resume() const
{
	impl->Resume();
}

void glib::Animation::Stop() const
{
	impl->Stop();
}

bool glib::Animation::IsFinished() const
{
	return impl->IsFinished();
}

void glib::Animation::SetFPS(int fps)
{
	impl->SetFPS(fps);
}

void glib::Animation::SetLooping(bool looping)
{
	impl->SetLooping(looping);
}

void glib::Animation::SetOffset(const Vec2& offset)
{
	impl->SetOffset(offset);
}

void glib::Animation::Update(float delta)
{
	impl->Update(delta);
}

std::map<std::string, Animation*> glib::Animation::Copy(const std::map<std::string, Animation*>& table)
{
	AnimationTable newTable;

	for (const auto& v : table)
	{
		std::vector<AnimationFrame> frames;
		for (AnimationFrame& frame : v.second->impl->m_Frames)
		{
			frame.tex->SetDeleteID(false);
			AnimationFrame f = frame;
			f.tex = new Texture(frame.tex->GetID(), frame.tex->width, frame.tex->height);
			frames.push_back(f);
		}

		Animation* ptr = new Animation(frames, v.second->impl->m_FPS, v.second->impl->m_Loop);
		newTable.insert({ v.first, ptr });
	}

	return newTable;
}

void glib::Animation::FreeTable(const std::map<std::string, Animation*>& table, bool alsoTextures)
{
	for (const auto& v : table)
	{
		v.second->impl->m_FreeTextures = alsoTextures;
		delete v.second;
	}
}
