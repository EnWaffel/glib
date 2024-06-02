#pragma once

#include "../../DLLDefs.h"
#include "../Animation.h"
#include "../../window/Window.h"

#include <string>
#include <map>

namespace glib
{
	class SparrowAtlasLoader
	{
	public:
		GLIB_API static std::map<std::string, Animation*> LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped);
	};
}