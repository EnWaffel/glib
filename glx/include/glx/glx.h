#pragma once
#include <glib/glib.h>
#include "State.h"


/**
* glx is an extension of glib that is trying to imitate HaxeFlixel.
*/

namespace glx
{
	using namespace glib;
	
	void Init(const State& initialState, const std::string& title, int width, int height);
	void SwitchState(State* state);

	Texture* LoadTexture(const std::string& path);
	void CenterSprite(Axis axis, Sprite& sprite);
}