#include "glib/graphics/pipeline/CameraRenderer.h"
#include <vector>
#include "glib/graphics/camera/Camera.h"
#include "glib/window/Window.h"
#include "glib/graphics/Sprite.h"
#include <iostream>

#include <memory>
#include <glad/glad.h>

using namespace glib;

static const char* VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 _texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	texCoord = _texCoord;
}
)";

static const char* FRAGMENT_SADER = R"(
#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform bool xFlip;
uniform bool yFlip;

in vec2 texCoord;

void main()
{
	vec2 coord = texCoord;
	if (xFlip)
	{
		coord.x = -coord.x;
	}
	gl_FragColor = texture(tex, coord) * color;
}
)";

namespace glib
{
	static class Listener : public EventSubscriber
	{
	private:
		CameraRenderer* m_CR;
	public:
		Listener(CameraRenderer* wr) : m_CR(wr)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_CR->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::CameraRenderer::CameraRenderer()
{
	m_L = new Listener(this);
}

glib::CameraRenderer::~CameraRenderer()
{
	m_WND->GetEventManager().Unsubscribe(m_L);
	delete m_L;
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void glib::CameraRenderer::Construct(Window* wnd)
{
	m_WND = wnd;
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, m_L);

	Vec2 size = wnd->GetInitialSize();
	m_Size = size;

	m_Shd = wnd->LoadShader(VERTEX_SHADER, FRAGMENT_SADER);

	ConstructFBO(Vec2(0.0f, 0.0f), size);

	float vertices[] = {
		// first triangle
		1.0f,  1.0f, 0.0f, 1.0, 1.0,  // top right
		1.0f,  0.0f, 0.0f, 1.0, 0.0,  // bottom right
		0.0f,  1.0f, 0.0f, 0.0, 1.0, // top left
		// second triangle
		1.0f,  0.0f, 0.0f, 1.0, 0.0, // bottom right
		0.0f,  0.0f, 0.0f, 0.0, 0.0, // bottom left
		0.0f,  1.0f, 0.0f,  0.0, 1.0 // top left
	};

	int verticeCount = 6;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticeCount * 3 + sizeof(float) * verticeCount * 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

const PipelineData glib::CameraRenderer::Downstream(const PipelineData data)
{
	Camera* cam = (Camera*) data.ptr;

	m_FB->Bind();
	glViewport(0, 0, m_Size.x, m_Size.y);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shd->Use();
	Mat4 m = cam->CalculateView();
	m_Shd->SetMat4("view", m);

	for (Drawable* d : cam->GetDrawables())
	{
		if (!d->visible) continue;
		d->Draw();
		Sprite* s = dynamic_cast<Sprite*>(d);
		if (!s) continue;

		Mat4 modelMat;
		Vec2 diff = Vec2((s->size.x * s->scale.x) - s->size.x, (s->size.y * s->scale.y) - s->size.y);

		modelMat.Translate(s->offset);
		modelMat.Translate(Vec2(s->pos.x - (diff.x / 2.0f), s->pos.y - (diff.y / 2.0f)));

		modelMat.Translate(Vec2(s->size.x / 2.0f * s->scale.x, s->size.y / 2.0f * s->scale.y));
		modelMat.Rotate(s->rotation);
		modelMat.Translate(Vec2(-(s->size.x / 2.0f) * s->scale.x, -(s->size.y / 2.0f) * s->scale.y));

		if (s->xFlip)
		{
			modelMat.Scale(Vec2(-s->size.x + -diff.x, s->size.y + diff.y));
		}
		else
		{
			modelMat.Scale(Vec2(s->size.x + diff.x, s->size.y + diff.y));
		}


		m_Shd->SetMat4("model", modelMat);
		m_Shd->SetColor("color", s->color);

		if (s->tex != nullptr)
		{
			m_Shd->SetInt("tex", 0);
			glActiveTexture(GL_TEXTURE0);
			s->tex->Bind();
		}

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		if (s->tex != nullptr)
		{
			s->tex->Unbind();
		}
	}

	m_FB->Unbind();

	return { cam, m_FB->GetInternals().tex };
}

void glib::CameraRenderer::ConstructFBO(Vec2 pos, Vec2 size)
{
	m_WND->SetToCurrentContext();
	if (m_FB != nullptr)
	{
		delete m_FB;
	}

	m_Size = size;
	m_Pos = pos;

	Mat4 m = Mat4::Ortho(0.0f, m_WND->GetInitialSize().x, m_WND->GetInitialSize().y, 0.0f);
	m_Shd->Use();
	m_Shd->SetMat4("projection", m);

	m_FB = new Framebuffer(FramebufferType::TEXTURE, size);
}
