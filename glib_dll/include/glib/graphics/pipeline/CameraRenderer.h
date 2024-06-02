#pragma once

#include "PipelineElement.h"
#include "../Shader.h"
#include "../../math/Mat4.h"
#include "../../framebuffer/Framebuffer.h"

namespace glib
{
	class Listener;

	class CameraRenderer : public PipelineElement
	{
	private:
		Shader* m_Shd;
		unsigned int m_VAO;
		unsigned int m_VBO;
		Framebuffer* m_FB;
		Vec2 m_Size;
		Vec2 m_Pos;
		Listener* m_L;
		Window* m_WND;
		Mat4 m_Proj;
	public:
		CameraRenderer();
		~CameraRenderer();

		void Construct(Window* wnd);
		const PipelineData Downstream(const PipelineData data);
		void ConstructFBO(Vec2 pos, Vec2 size);
	};
}