#include "glib/graphics/Sprite.h"
#include "glib/utils/Easing.h"
#include "glib/animation/AnimationManager.h"

#include <iostream>
#include <tweeny.h>

extern tweeny::easing::enumerated __translate_easing(glib::Easing easing);

namespace glib
{
	class SpriteImpl : public Animatable
	{
	private:
		Sprite* m_Sprite;
		tweeny::tween<float, float> m_PosTween;
		bool m_PosTweenActive = false;
		tweeny::tween<float, float> m_SizeTween;
		bool m_SizeTweenActive = false;
		tweeny::tween<float, float> m_ScaleTween;
		bool m_ScaleTweenActive = false;
		AnimationManager m_AnimationManager;
	public:
		SpriteImpl(Sprite* s) : m_Sprite(s), m_AnimationManager(this)
		{
		}

		void Update(float delta)
		{
			m_AnimationManager.Update(delta);

			if (m_PosTweenActive)
			{
				if (m_PosTween.progress() >= 1.0f)
				{
					m_PosTweenActive = false;
				}
				m_PosTween.step((int)delta);
			}
			if (m_SizeTweenActive)
			{
				if (m_SizeTween.progress() >= 1.0f)
				{
					m_SizeTweenActive = false;
				}
				m_SizeTween.step((int)delta);
			}
			if (m_ScaleTweenActive)
			{
				if (m_ScaleTween.progress() >= 1.0f)
				{
					m_ScaleTweenActive = false;
				}
				m_ScaleTween.step((int)delta);
			}
		}

		void TweenPosition(const Vec2& to, float time, const Easing& easing)
		{
			m_PosTweenActive = true;
			m_PosTween = tweeny::from(m_Sprite->pos.x, m_Sprite->pos.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->pos.x = x;
				m_Sprite->pos.y = y;
				return false;
			});
		}

		void TweenSize(const Vec2& to, float time, const Easing& easing)
		{
			m_SizeTweenActive = true;
			m_SizeTween = tweeny::from(m_Sprite->size.x, m_Sprite->size.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->size.x = x;
				m_Sprite->size.y = y;
				return false;
			});
		}

		void TweenScale(const Vec2& to, float time, const Easing& easing)
		{
			m_ScaleTweenActive = true;
			m_ScaleTween = tweeny::from(m_Sprite->scale.x, m_Sprite->scale.y).to(to.x, to.y).during(time).via(__translate_easing(easing)).onStep([this](float x, float y) {
				m_Sprite->scale.x = x;
				m_Sprite->scale.y = y;
				return false;
			});
		}

		void SetValues(const AnimValues& values) override
		{
			m_Sprite->offset.x = values.x * m_Sprite->scale.x;
			m_Sprite->offset.y = values.y * m_Sprite->scale.y;
			m_Sprite->SetTexture(values.tex);
			m_Sprite->size.x += values.w;
			m_Sprite->size.y += values.h;
		}

		const AnimValues GetValues() override
		{
			return {};
		}

		void AddAnimation(const std::string& name, Animation* animation)
		{
			m_AnimationManager.AddAnimation(name, animation);
		}

		void PlayAnimation(const std::string& animationName)
		{
			m_AnimationManager.PlayAnimation(animationName);
		}

		void AddAllAnimations(const std::map<std::string, Animation*> animations)
		{
			m_AnimationManager.AddAll(animations);
		}

		Animation* GetAnimation(const std::string& name)
		{
			return m_AnimationManager.GetAnimation(name);
		}

		void SetDefaultAnimation(Animation* animation)
		{
			m_AnimationManager.SetDefault(animation);
		}

		void SetDefaultAnimation(const std::string& name)
		{
			m_AnimationManager.SetDefault(name);
		}

		const Animation* GetCurrentAnimation() const
		{
			return m_AnimationManager.GetCurrentAnimation();
		}

		const std::string& GetCurrentAnimationName() const
		{
			return m_AnimationManager.GetCurrentAnimationName();
		}
	};
}

using namespace glib;

glib::Sprite::Sprite() : scale(Vec2(1.0f, 1.0f)), tex(nullptr), rotation(0.0f), color({ 1.0f, 1.0f, 1.0f, 1.0f }), xFlip(false), yFlip(false)
{
	impl = new SpriteImpl(this);
	visible = true;
}

glib::Sprite::~Sprite()
{
	delete impl;
}

void glib::Sprite::Center(const Axis& axis, const Vec2& containerSize)
{
	switch (axis)
	{
	case Axis::X:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		break;
	}
	case Axis::Y:
	{
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	case Axis::XY:
	{
		pos.x = containerSize.x / 2.0f - size.x / 2.0f;
		pos.y = containerSize.y / 2.0f - size.y / 2.0f;
		break;
	}
	}
}

void glib::Sprite::SetTexture(Texture* tex)
{
	size = Vec2(tex->width, tex->height);
	this->tex = tex;
}

void glib::Sprite::Draw()
{
	// Do nothing.
}

void glib::Sprite::Update(float delta)
{
	impl->Update(delta);
}

void glib::Sprite::TweenPosition(const Vec2& to, float time, const Easing& easing)
{
	impl->TweenPosition(to, time, easing);
}

void glib::Sprite::TweenSize(const Vec2& to, float time, const Easing& easing)
{
	impl->TweenSize(to, time, easing);
}

void glib::Sprite::TweenScale(const Vec2& to, float time, const Easing& easing)
{
	impl->TweenScale(to, time, easing);
}

void glib::Sprite::AddAnimation(const std::string& name, Animation* animation)
{
	impl->AddAnimation(name, animation);
}

void glib::Sprite::PlayAnimation(const std::string& animationName)
{
	impl->PlayAnimation(animationName);
}

void glib::Sprite::AddAllAnimations(const std::map<std::string, Animation*> animations)
{
	impl->AddAllAnimations(animations);
}

Animation* glib::Sprite::GetAnimation(const std::string& name)
{
	return impl->GetAnimation(name);
}

void glib::Sprite::SetDefaultAnimation(Animation* animation)
{
	impl->SetDefaultAnimation(animation);
}

void glib::Sprite::SetDefaultAnimation(const std::string& name)
{
	impl->SetDefaultAnimation(name);
}

const Animation* glib::Sprite::GetCurrentAnimation() const
{
	return impl->GetCurrentAnimation();
}

const std::string& glib::Sprite::GetCurrentAnimationName() const
{
	return impl->GetCurrentAnimationName();
}
