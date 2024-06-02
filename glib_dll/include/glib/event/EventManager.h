#pragma once

#include "../DLLDefs.h"
#include "Event.h"
#include "EventSubscriber.h"

namespace glib
{
	typedef int EventList;

	class EventManagerImpl;

	class EventManager
	{
	private:
		EventManagerImpl* impl;
	public:
		EventManager();
		~EventManager();

		GLIB_API void Subscribe(EventList events, EventSubscriber* subscriber);
		GLIB_API void Subscribe(EventList events, EventSubscriber& subscriber);
		GLIB_API void Unsubscribe(EventSubscriber* subscriber);
		GLIB_API void Unsubscribe(EventSubscriber& subscriber);
		GLIB_API void CallEvent(const Event& event);
	};
}