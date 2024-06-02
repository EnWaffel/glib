#pragma once

#include "../DLLDefs.h"
#include "Event.h"

namespace glib
{
	class EventSubscriber
	{
	public:
		GLIB_API virtual void OnKeyPress(const KeyEvent& event);
		GLIB_API virtual void OnKeyRelease(const KeyEvent& event);
		GLIB_API virtual void OnWindowResize(const WindowResizeEvent& event);
	};
}