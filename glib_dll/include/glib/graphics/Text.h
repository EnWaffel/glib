#pragma once

#include "../DLLDefs.h"
#include "Drawable.h"
#include "../math/Vec2.h"
#include "../utils/Color.h"
#include "../math/Axis.h"
#include "../utils/Easing.h"
#include "Font.h"

namespace glib
{
	class TextImpl;

	class Text : public Drawable
	{
	public:
		Vec2 pos;
		Vec2 size;
		float scale;
		Vec2 offset;
		float rotation;
		Color color;
		Font* font;
		bool xFlip;
		bool yFlip;
	private:
		TextImpl* impl;
	public:
		GLIB_API Text(Font* font);
		GLIB_API ~Text();

		GLIB_API void Center(const Axis& axis, const Vec2& containerSize);

		GLIB_API void Draw() override; // Does absolutley nothing.
		GLIB_API virtual void Update(float delta) override; // Does actually something.

		GLIB_API void TweenPosition(const Vec2& to, float time, const Easing& easing);
		GLIB_API void TweenSize(const Vec2& to, float time, const Easing& easing);
		GLIB_API void TweenScale(float to, float time, const Easing& easing);
	};
}