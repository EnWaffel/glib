#include "glib/event/EventManager.h"

#include <map>

namespace glib
{
	class EventManagerImpl
	{
	private:
		std::map<EventSubscriber* , EventList> m_Subscribers;
	public:
		EventManagerImpl()
		{
		}

		~EventManagerImpl()
		{
		}

		void Subscribe(EventList events, EventSubscriber* sub)
		{
			m_Subscribers.insert({ sub, events });
		}

		void Unsubscribe(EventSubscriber* sub)
		{
			m_Subscribers.erase(sub);
		}

		void CallEvent(const Event& event)
		{
			for (const auto& v : m_Subscribers)
			{
				if (v.second & GLIB_EVENT_KEY_PRESS && event.id == GLIB_EVENT_KEY_PRESS)
				{
					v.first->OnKeyPress((const KeyEvent&)event);
				}
				else if (v.second & GLIB_EVENT_KEY_RELEASE && event.id == GLIB_EVENT_KEY_RELEASE)
				{
					v.first->OnKeyRelease((const KeyEvent&)event);
				}
				else if (v.second & GLIB_EVENT_WINDOW_RESIZE && event.id == GLIB_EVENT_WINDOW_RESIZE)
				{
					v.first->OnWindowResize((const WindowResizeEvent&)event);
				}
			}
		}
	};
}

using namespace glib;

glib::EventManager::EventManager()
{
	impl = new EventManagerImpl;
}

glib::EventManager::~EventManager()
{
	delete impl;
}

void glib::EventManager::Subscribe(EventList events, EventSubscriber* subscriber)
{
	impl->Subscribe(events, subscriber);
}

void glib::EventManager::Subscribe(EventList events, EventSubscriber& subscriber)
{
	impl->Subscribe(events, &subscriber);
}

void glib::EventManager::Unsubscribe(EventSubscriber* subscriber)
{
	impl->Unsubscribe(subscriber);
}

void glib::EventManager::Unsubscribe(EventSubscriber& subscriber)
{
	impl->Unsubscribe(&subscriber);
}

void glib::EventManager::CallEvent(const Event& event)
{
	impl->CallEvent(event);
}
