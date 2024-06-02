#pragma once

#include "../DLLDefs.h"
#include <string>
#include "glib/math/Mat4.h"
#include "../utils/Color.h"

namespace glib
{
	class ShaderImpl;

	class Shader
	{
	private:
		ShaderImpl* impl;
	public:
		Shader(unsigned int id);
		~Shader();

		void Use() const;
		int GetUniformLocation(const std::string& name);
		void SetMat4(const std::string& name, Mat4& mat);
		void SetInt(const std::string& name, int i);
		void SetColor(const std::string& name, Color& color);
	};
}