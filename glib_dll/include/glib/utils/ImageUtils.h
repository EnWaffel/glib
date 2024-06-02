#pragma once

#include "../DLLDefs.h"
#include "../graphics/Texture.h"

namespace glib
{
	class ImageUtils
	{
	public:
		ImageUtils() = delete;
		~ImageUtils() = delete;

		static ImageData Cut(ImageData data, int x, int y, int width, int height);
	};
}