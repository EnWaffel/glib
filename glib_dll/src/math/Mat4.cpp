#include "glib/math/Mat4.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace glib
{
	class Mat4Impl
	{
	public:
		glm::mat4 mat;
	public:
		Mat4Impl()
		{
			mat = glm::mat4(1.0f);
		}

		Mat4Impl(const glm::mat4& m)
		{
			mat = m;
		}

		void Translate(Vec2& vec)
		{
			mat = glm::translate(mat, glm::vec3(vec.x, vec.y, 0.0f));
		}

		void Scale(Vec2& vec)
		{
			mat = glm::scale(mat, glm::vec3(vec.x, vec.y, 0.0f));
		}

		void Rotate(float deg)
		{
			mat = glm::rotate(mat, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		void* GetPtr()
		{
			return &mat;
		}

		Mat4 Mult(Mat4& other)
		{
			Mat4 m;
			m.impl->mat = mat * other.impl->mat;
			return m;
		}
	};
}

using namespace glib;

glib::Mat4::Mat4()
{
	impl = new Mat4Impl;
}

glib::Mat4::~Mat4()
{
	delete impl;
}

void glib::Mat4::Translate(Vec2 vec)
{
	impl->Translate(vec);
}

void glib::Mat4::Scale(Vec2 vec)
{
	impl->Scale(vec);
}

void glib::Mat4::Rotate(float deg)
{
	impl->Rotate(deg);
}

Mat4 glib::Mat4::operator*(Mat4& other)
{
	return impl->Mult(other);
}

void* glib::Mat4::GetPtr()
{
	return impl->GetPtr();
}

Mat4 glib::Mat4::Copy()
{
	Mat4 m;
	m.impl->mat = impl->mat;
	return m;
}

Mat4 glib::Mat4::Ortho(float left, float right, float bottom, float top)
{
	Mat4 m;
	m.impl->mat = glm::ortho(left, right, bottom, top);

	return m;
}
