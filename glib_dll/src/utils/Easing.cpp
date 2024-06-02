#include "glib/utils/Easing.h"

#include <tweeny.h>

using namespace glib;

tweeny::easing::enumerated __translate_easing(Easing easing)
{
	int i = (int)easing;
	return (tweeny::easing::enumerated)i;
}