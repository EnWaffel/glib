#include "glx/glx.h"
#include <iostream>

using namespace glx;

static Instance* instance;
static Window* window;
static State* currentState = nullptr;
static bool requestStateChange;
static State* requestedState = nullptr;

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

class EventListener : public glib::EventSubscriber
{
public:
	void OnKeyPress(const KeyEvent& event) override
	{
		if (currentState == nullptr) return;
		currentState->OnKeyPress(event.key);
	}

	void OnKeyRelease(const KeyEvent& event) override
	{
		if (currentState == nullptr) return;
		currentState->OnKeyRelease(event.key);
	}
};

void glx::Init(const State& initialState, const std::string& title, int width, int height)
{
	instance = glib::Init();
	window = instance->CreateWindow(title, width, height);

	EventListener l;
	window->GetEventManager().Subscribe(GLIB_EVENT_ALL, l);

	currentState = (State*) &initialState;
	currentState->_Enable(window);
	currentState->Enable();

	window->SetVisible(true);

	double last = instance->GetTime() * 1000.0f;
	while (window->IsOpen())
	{
		double now = instance->GetTime() * 1000.0f;
		if (now >= last + 1000.0f / 60.0f)
		{
			float delta = now - last;
			last = now;

			if (requestStateChange)
			{
				requestStateChange = false;

				if (currentState != nullptr)
				{
					currentState->Disable();
					window->GetDefaultCamera()->RemoveAll();
				}
				currentState = requestedState;
				currentState->_Enable(window);
				currentState->Enable();
			}

			window->UpdateEvents(delta);

			if (currentState != nullptr)
			{
				currentState->Update(delta);
			}

			window->Update(delta);
			window->Draw();
		}
	}

	glib::Terminate();
}

void glx::SwitchState(State* state)
{
	requestStateChange = true;
	requestedState = state;
}

Texture* glx::LoadTexture(const std::string& path)
{
	return window->LoadTexture(path);
}

void glx::CenterSprite(Axis axis, Sprite& sprite)
{
	sprite.Center(axis, Vec2(window->GetInitialSize()));
}
