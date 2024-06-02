#pragma once

#include "../DLLDefs.h"
#include "Drawable.h"
#include "../math/Vec2.h"
#include "Texture.h"
#include "../utils/Easing.h"
#include "../math/Axis.h"
#include "../utils/Color.h"
#include "../animation/Animation.h"

#include <string>
#include <map>

namespace glib
{
	class SpriteImpl;

	class Sprite : public Drawable
	{
	public:
		Vec2 pos;
		Vec2 size;
		Vec2 scale;
		Vec2 offset;
		Texture* tex;
		float rotation;
		Color color;
		bool xFlip;
		bool yFlip;
	private:
		SpriteImpl* impl;
	public:
		GLIB_API Sprite();
		GLIB_API ~Sprite();

		GLIB_API void Center(const Axis& axis, const Vec2& containerSize);
		GLIB_API void SetTexture(Texture* tex);

		GLIB_API void Draw() override; // Does absolutley nothing.
		GLIB_API virtual void Update(float delta) override; // Does actually something.

		GLIB_API void TweenPosition(const Vec2& to, float time, const Easing& easing);
		GLIB_API void TweenSize(const Vec2& to, float time, const Easing& easing);
		GLIB_API void TweenScale(const Vec2& to, float time, const Easing& easing);

		GLIB_API void AddAnimation(const std::string& name, Animation* animation);
		GLIB_API void PlayAnimation(const std::string& animationName);
		GLIB_API void AddAllAnimations(const std::map<std::string, Animation*> animations);
		GLIB_API Animation* GetAnimation(const std::string& name);
		GLIB_API void SetDefaultAnimation(Animation* animation);
		GLIB_API void SetDefaultAnimation(const std::string& name);
		GLIB_API const Animation* GetCurrentAnimation() const;
		GLIB_API const std::string& GetCurrentAnimationName() const;
	};
}