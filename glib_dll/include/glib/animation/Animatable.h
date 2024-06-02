#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"

namespace glib
{
	class AnimationImpl;

	struct GLIB_API AnimValues
	{
		Texture* tex;
		float x;
		float y;
		float w;
		float h;
	};

	class Animatable
	{
	protected:
		GLIB_API virtual void SetValues(const AnimValues& values) = 0;
		GLIB_API virtual const AnimValues GetValues() = 0;

		friend class AnimationImpl;
	};
}