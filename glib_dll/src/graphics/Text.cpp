#include "glib/graphics/Text.h"

namespace glib
{
	class TextImpl
	{
	public:
		TextImpl()
		{

		}

		~TextImpl()
		{

		}
	};
}

using namespace glib;

glib::Text::Text(Font* font)
{
	impl = new TextImpl;
}

glib::Text::~Text()
{
	delete impl;
}

void glib::Text::Center(const Axis& axis, const Vec2& containerSize)
{
}

void glib::Text::Draw()
{
}

void glib::Text::Update(float delta)
{
}

void glib::Text::TweenPosition(const Vec2& to, float time, const Easing& easing)
{
}

void glib::Text::TweenSize(const Vec2& to, float time, const Easing& easing)
{
}

void glib::Text::TweenScale(float to, float time, const Easing& easing)
{
}
