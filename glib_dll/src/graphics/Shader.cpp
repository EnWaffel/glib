#include "glib/graphics/Shader.h"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace glib
{
	class ShaderImpl
	{
	private:
		GLuint m_ID;
	public:
		ShaderImpl(GLuint id) : m_ID(id)
		{
		}

		~ShaderImpl()
		{
			glDeleteProgram(m_ID);
		}
		
		void Use() const
		{
			glUseProgram(m_ID);
		}

		int GetUniformLocation(const std::string& name)
		{
			return glGetUniformLocation(m_ID, name.c_str());
		}

		void SetMat4(const std::string& name, Mat4& mat)
		{
			glm::mat4 m = *((glm::mat4*)mat.GetPtr());
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
		}

		void SetInt(const std::string& name, int i)
		{
			glUniform1i(GetUniformLocation(name), i);
		}

		void SetColor(const std::string& name, Color& color)
		{
			glUniform4f(GetUniformLocation(name), color.r, color.g, color.b, color.a);
		}
	};
}

using namespace glib;

glib::Shader::Shader(unsigned int id)
{
	impl = new ShaderImpl(id);
}

glib::Shader::~Shader()
{
	delete impl;
}

void glib::Shader::Use() const
{
	impl->Use();
}

int glib::Shader::GetUniformLocation(const std::string& name)
{
	return impl->GetUniformLocation(name);
}

void glib::Shader::SetMat4(const std::string& name, Mat4& mat)
{
	impl->SetMat4(name, mat);
}

void glib::Shader::SetInt(const std::string& name, int i)
{
	impl->SetInt(name, i);
}

void glib::Shader::SetColor(const std::string& name, Color& color)
{
	impl->SetColor(name, color);
}
