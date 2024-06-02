#pragma once

#include <vector>

#include "../../DLLDefs.h"
#include "../camera/Camera.h"
#include "PipelineElement.h"

namespace glib
{
	class Window;

	class RenderPipeline
	{
	private:
		std::vector<PipelineElement*> m_Elements;
		Window* m_Wnd;
	public:
		GLIB_API RenderPipeline(Window* wnd);
		GLIB_API ~RenderPipeline();

		GLIB_API void Flush(const std::vector<Camera*>& cameras) const;
		GLIB_API void AddElement(PipelineElement* element);
		GLIB_API void RemoveElement(PipelineElement* element);
	};
}