#pragma once

#include "../DLLDefs.h"
#include "Animatable.h"
#include "Animation.h"

#include <string>
#include <map>

namespace glib
{
	class AnimationManagerImpl;

	class AnimationManager
	{
	private:
		AnimationManagerImpl* impl;
	public:
		GLIB_API AnimationManager(Animatable* obj);
		GLIB_API ~AnimationManager();

		GLIB_API void PlayAnimation(const std::string& animationName);
		GLIB_API void AddAnimation(const std::string& name, Animation* animation);
		GLIB_API void AddAll(const std::map<std::string, Animation*> animations);
		GLIB_API Animation* GetAnimation(const std::string& name);
		GLIB_API void SetDefault(Animation* animation);
		GLIB_API void SetDefault(const std::string& name);
		GLIB_API const Animation* GetCurrentAnimation() const;
		GLIB_API const std::string& GetCurrentAnimationName() const;

		void Update(float delta);
	};
}