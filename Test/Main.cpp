#include <iostream>
#include <glib/glib.h>

static bool wPressed = false;
static bool aPressed = false;
static bool sPressed = false;
static bool dPressed = false;

class Listener : public glib::EventSubscriber
{
	void OnKeyPress(const glib::KeyEvent& event)
	{
		if (event.key == glib::KeyCode::W)
		{
			wPressed = true;
		}
		if (event.key == glib::KeyCode::A)
		{
			aPressed = true;
		}
		if (event.key == glib::KeyCode::S)
		{
			sPressed = true;
		}
		if (event.key == glib::KeyCode::D)
		{
			dPressed = true;
		}
	}

	void OnKeyRelease(const glib::KeyEvent& event)
	{
		if (event.key == glib::KeyCode::W)
		{
			wPressed = false;
		}
		if (event.key == glib::KeyCode::A)
		{
			aPressed = false;
		}
		if (event.key == glib::KeyCode::S)
		{
			sPressed = false;
		}
		if (event.key == glib::KeyCode::D)
		{
			dPressed = false;
		}
	}
};

int main()
{
	glib::Instance* instance = glib::Init();
	glib::Window* wnd = instance->CreateWindow("Test", 800, 600);

	glib::Texture* bgTex = wnd->LoadTexture("bg.png");
	glib::Texture* gndTex = wnd->LoadTexture("ground.png");
	glib::Texture* plrTex = wnd->LoadTexture("player.png");

	Listener l;
	wnd->GetEventManager().Subscribe(GLIB_EVENT_KEY_PRESS | GLIB_EVENT_KEY_RELEASE, l);

	glib::Sprite bg;
	bg.SetTexture(bgTex);

	glib::Sprite gnd;
	gnd.SetTexture(gndTex);
	gnd.pos.y = 500;

	glib::Sprite player;
	player.SetTexture(plrTex);
	player.scale = glib::Vec2(0.15f, 0.15f);

	wnd->GetDefaultCamera()->Add(bg);
	wnd->GetDefaultCamera()->Add(gnd);
	wnd->GetDefaultCamera()->Add(player);

	//wnd->GetDefaultCamera()->zoom = 1.5f;

	wnd->SetVisible(true);
	double last = instance->GetTime() * 1000.0f;
	while (wnd->IsOpen())
	{
		double now = instance->GetTime() * 1000.0f;
		if (now >= last + 1000.0f / 60.0f)
		{
			float delta = now - last;
			last = now;
			wnd->UpdateEvents(delta);

			if (wPressed)
			{
				player.pos.y -= 0.5f * delta;
			}
			if (aPressed)
			{
				player.pos.x -= 0.5f * delta;
			}
			if (sPressed)
			{
				player.pos.y += 0.5f * delta;
			}
			if (dPressed)
			{
				player.pos.x += 0.5f * delta;
			}

			wnd->Update(delta);
			wnd->Draw();
		}
	}

	glib::Terminate();
}