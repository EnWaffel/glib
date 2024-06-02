#pragma once

#include <glib/window/Window.h>
#include <glib/graphics/Drawable.h>

using namespace glib;

namespace glx
{
	class StateImpl;

	class State
	{
	private:
		StateImpl* impl;
	public:
		State();
		~State();

		void _Enable(Window* wnd);

		virtual void Enable() = 0;
		virtual void Update(float delta) = 0;
		virtual void Disable() = 0;

		virtual void OnKeyPress(KeyCode key) = 0;
		virtual void OnKeyRelease(KeyCode key) = 0;
	protected:
		void Add(Drawable* drawable);
		void Add(Drawable& drawable);
		void Remove(Drawable* drawable);
		void Remove(Drawable& drawable);
	};
}