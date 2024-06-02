#include "glib/glib.h"
#include <iostream>

using namespace glib;

static Instance* instance = nullptr;
int __GLIB_ERROR_CODE = -1;

void glib_print_error()
{
	if (__GLIB_ERROR_CODE == -1)
	{
		std::cout << "glib Error: INVALID ERROR CODE" << std::endl;
	}
	else
	{
		std::cout << "glib Error: 0x" << std::hex << __GLIB_ERROR_CODE << std::endl;
	}
	__GLIB_ERROR_CODE = -1;
}

Instance* glib::Init()
{
	if (instance != nullptr) return instance;
	instance = new Instance;

	if (__GLIB_ERROR_CODE != 0)
	{
		glib_print_error();
		delete instance;
		return nullptr;
	}

	return instance;
}

void glib::Terminate()
{
	if (instance == nullptr) return;
	delete instance;
}
