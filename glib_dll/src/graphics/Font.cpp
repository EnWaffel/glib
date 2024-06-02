#include "glib/graphics/Font.h"

namespace glib
{
	class FontImpl
	{
	public:
		FontImpl()
		{

		}

		~FontImpl()
		{

		}
	};
}

using namespace glib;

glib::Font::Font()
{
	impl = new FontImpl;
}

glib::Font::~Font()
{
	delete impl;
}
