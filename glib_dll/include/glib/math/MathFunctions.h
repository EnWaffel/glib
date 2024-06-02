#pragma once

#include "Vec2.h"
#include "../DLLDefs.h"

namespace glib
{
	GLIB_API float Lerp(float a, float b, float f);
	GLIB_API float Lerp(const Vec2& vec, float f);
}