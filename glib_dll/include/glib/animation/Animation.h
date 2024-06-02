#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"
#include "Animatable.h"
#include "../math/Vec2.h"

#include <vector>
#include <string>
#include <map>

namespace glib
{
	struct GLIB_API AnimationFrame
	{
		Texture* tex;
		float x;
		float y;
		float w;
		float h;
	};

	class AnimationImpl;

	class Animation
	{
	private:
		AnimationImpl* impl;
	public:
		GLIB_API Animation(std::vector<AnimationFrame> frames, int fps, bool loop);
		GLIB_API ~Animation();

		GLIB_API void Play(Animatable* obj) const;
		GLIB_API void Pause() const;
		GLIB_API void Resume() const;
		GLIB_API void Stop() const;
		GLIB_API bool IsFinished() const;

		GLIB_API void SetFPS(int fps);
		GLIB_API void SetLooping(bool looping);
		GLIB_API void SetOffset(const Vec2& offset);

		void Update(float delta);

		GLIB_API static std::map<std::string, Animation*> Copy(const std::map<std::string, Animation*>& table);
		GLIB_API static void FreeTable(const std::map<std::string, Animation*>& table, bool alsoTextures = true);
	};

	typedef std::map<std::string, Animation*> AnimationTable;
}