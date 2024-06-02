#pragma once

#include "../../math/Vec2.h"

namespace glib
{
	class Window;

	struct PipelineData
	{
		const void* ptr;
		unsigned int uI;
		int i;
		float f;
		Window* wnd;
	};

	class PipelineElement
	{
	public:
		Vec2 viewportPos;
		Vec2 viewportSize;
	public:
		virtual void Construct(Window* wnd) = 0;
		virtual const PipelineData Downstream(const PipelineData data) = 0;
	};
}