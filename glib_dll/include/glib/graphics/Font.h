#pragma once

#include "../DLLDefs.h"

namespace glib
{
	class FontImpl;

	class Font
	{
	private:
		FontImpl* impl;
	public:
		Font();
		~Font();
	};

}