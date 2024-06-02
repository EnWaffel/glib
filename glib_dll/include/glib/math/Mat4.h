#pragma once

#include "../DLLDefs.h"
#include "Vec2.h"

namespace glib
{
	class Mat4Impl;

	class Mat4
	{
	private:
		Mat4Impl* impl;
	public:
		GLIB_API Mat4();
		GLIB_API ~Mat4();

		GLIB_API void Translate(Vec2 vec);
		GLIB_API void Scale(Vec2 vec);
		GLIB_API void Rotate(float deg);

		GLIB_API Mat4 operator*(Mat4& other);
			
		GLIB_API void* GetPtr();
		GLIB_API Mat4 Copy();

		friend class Mat4Impl;
	public:
		GLIB_API static Mat4 Ortho(float left, float right, float bottom, float top);
	};
}