#pragma once

#ifdef GLIBDLL_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif