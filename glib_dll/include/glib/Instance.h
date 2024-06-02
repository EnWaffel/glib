#pragma once

#include <string>

#include "DLLDefs.h"
#include "window/Window.h"
#include "glibError.h"

namespace glib
{
	class InstanceImpl;

	class Instance
	{
	private:
		InstanceImpl* impl;
	public:
		Instance();
		~Instance();

		GLIB_API Window* CreateWindow(const std::string& title, int width, int height);
		GLIB_API RenderPipeline* CreateDefaultPipeline(Window* wnd) const;
		GLIB_API double GetTime() const;
	};
}