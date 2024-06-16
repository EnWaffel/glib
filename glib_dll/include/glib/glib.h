#pragma once
#include <string>

#include "DLLDefs.h"
#include "Instance.h"
#include "graphics/Sprite.h"
#include "graphics/camera/Camera.h"
#include "graphics/Text.h"
#include "physics/PhysicsManager.h"

/**
* glib (game library) is a game library aiming to make it easier to create games with c++.
*/
namespace glib
{
	GLIB_API Instance* Init();
	GLIB_API void Terminate();
}