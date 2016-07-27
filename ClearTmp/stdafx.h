// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <windows.h>
#define RUN_TEST
#undef __TEXT
#undef TEXT

#ifdef _UNICODE
#define t_string std::wstring
#define __TEXT(quote) L##quote

#else
#define t_string std::string
#define __TEXT(quote) quote
#endif 

#define TEXT(quote) __TEXT(quote)

namespace ClearTmp
{
    namespace util
    {
        t_string NormallizeSearchPath(const t_string& path);
        t_string FormatErrorMessage();
    }
}