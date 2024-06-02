#include "glx/State.h"

using namespace glx;
using namespace glib;

namespace glx
{
	class StateImpl
	{
	public:
		Window* m_WND;
	public:
		StateImpl()
		{

		}

		~StateImpl()
		{

		}

		void _Enable(Window* wnd)
		{
			m_WND = wnd;
		}

		void Add(Drawable* drawable)
		{
			m_WND->GetDefaultCamera()->Add(drawable);
		}

		void Add(Drawable& drawable)
		{
			m_WND->GetDefaultCamera()->Add(drawable);
		}

		void Remove(Drawable* drawable)
		{
			m_WND->GetDefaultCamera()->Remove(drawable);
		}

		void Remove(Drawable& drawable)
		{
			m_WND->GetDefaultCamera()->Remove(drawable);
		}
	};
}


glx::State::State()
{
	impl = new StateImpl;
}

glx::State::~State()
{
	delete impl;
}

void glx::State::_Enable(Window* wnd)
{
	impl->_Enable(wnd);
}

void State::Add(Drawable* drawable)
{
	impl->Add(drawable);
}

void State::Add(Drawable& drawable)
{
	impl->Add(drawable);
}

void State::Remove(Drawable* drawable)
{
	impl->Remove(drawable);
}

void State::Remove(Drawable& drawable)
{
	impl->Remove(drawable);
}
