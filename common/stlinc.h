#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <windows.h>


#ifdef _UNICODE
#define t_string std::wstring
#define __TEXT(quote) L##quote

#else
#define t_string std::string
#define __TEXT(quote) quote
#endif 

#define TEXT(quote) __TEXT(quote)


namespace Common
{
	namespace util
	{
		t_string NormallizeSearchPath(const t_string& path);
		t_string FormatErrorMessage();
	}
}