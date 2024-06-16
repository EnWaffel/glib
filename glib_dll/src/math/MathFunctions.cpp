#include "glib/math/MathFunctions.h"

float glib::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float glib::Lerp(const Vec2& vec, float f)
{
    return vec.x + f * (vec.y - vec.x);
}

bool glib::LineIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1)
{
    return ((lp1 >= rP1 && lp1 <= (rP1 + rS1)) && (lp1_1 >= rP2 && lp1_1 <= (rP2 + rS2))) || ((lp2 >= rP1 && lp2 <= (rP1 + rS1)) && (lp2_1 >= rP2 && lp2_1 <= (rP2 + rS2)));
}
