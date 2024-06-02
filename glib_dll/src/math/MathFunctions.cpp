#include "glib/math/MathFunctions.h"

float glib::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float glib::Lerp(const Vec2& vec, float f)
{
    return vec.x + f * (vec.y - vec.x);
}
