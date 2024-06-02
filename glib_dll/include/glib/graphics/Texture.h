#pragma once

#include "../DLLDefs.h"

namespace glib
{
	struct GLIB_API ImageData {
		int channels;
		int width;
		int height;
		unsigned char* data;
	};

	class TextureImpl;

	class Texture
	{
	private:
		TextureImpl* impl;
	public:
		int width;
		int height;
	public:
		Texture(unsigned int id, int width, int height);
		~Texture();

		GLIB_API void Bind();
		GLIB_API void Unbind();
		GLIB_API unsigned int GetID();

		void SetDeleteID(bool deleteID);
	};
}