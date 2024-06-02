#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class Vec2
	{
	public:
		float x;
		float y;
	public:
		GLIB_API Vec2();
		GLIB_API Vec2(float x, float y);
		GLIB_API Vec2 operator+(Vec2& other);
		GLIB_API Vec2 operator-(Vec2& other);
		GLIB_API Vec2 operator+(float value);
		GLIB_API Vec2 operator-(float value);
		GLIB_API void operator+=(float value);
		GLIB_API void operator-=(float value);
		GLIB_API Vec2 operator-();
	};
}