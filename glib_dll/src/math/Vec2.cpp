#include "glib/math/Vec2.h"

using namespace glib;

glib::Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

glib::Vec2::Vec2(float _x, float _y) : x(_x), y(_y)
{
}

Vec2 glib::Vec2::operator+(Vec2& other)
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 glib::Vec2::operator-(Vec2& other)
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 glib::Vec2::operator+(float value)
{
    return Vec2(x + value, y + value);
}

Vec2 glib::Vec2::operator-(float value)
{
    return Vec2(x - value, y - value);
}

void glib::Vec2::operator+=(float value)
{
    x += value;
    y += value;
}

void glib::Vec2::operator-=(float value)
{
    x -= value;
    y -= value;
}

Vec2 glib::Vec2::operator-()
{
    return Vec2(-x, -y);
}
