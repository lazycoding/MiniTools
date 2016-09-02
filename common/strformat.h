#ifndef __STR_STR_HPP_INCLUDED_
#define __STR_STR_HPP_INCLUDED_
#include <vector>
#include <string>
#include <stdarg.h>
#include <cassert>

namespace lzns
{
	namespace str
	{
		//字符串格式化函数
		std::string format(const char *fmt, ...)
		{
			std::string strResult = "";
			if (NULL != fmt)
			{
				va_list marker = NULL;
				va_start(marker, fmt);                            //初始化变量参数 
				size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度
				std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
				int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
				if (nWritten > 0)
				{
					strResult = &vBuffer[0];
				}
				va_end(marker);                                    //重置变量参数
			}
			return strResult;
		}

		//字符串格式化函数
		std::wstring format(const wchar_t *fmt, ...)
		{
			std::wstring strResult = L"";
			if (NULL != fmt)
			{
				va_list marker = NULL;
				va_start(marker, fmt);                            //初始化变量参数
				size_t nLength = _vscwprintf(fmt, marker) + 1;    //获取格式化字符串长度
				std::vector<wchar_t> vBuffer(nLength, L'\0');    //创建用于存储格式化字符串的字符数组
				int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
				if (nWritten > 0)
				{
					strResult = &vBuffer[0];
				}
				va_end(marker);                                    //重置变量参数
			}
			return strResult;
		}

		int split(const std::string& orign, std::vector<std::string>& splited_strings, const std::string& spliter)
		{
			std::string::size_type off = 0;
			std::string::size_type find_position = orign.find(spliter, off);
			std::string stemp;
			while (find_position != std::string::npos)
			{
				stemp = orign.substr(off, find_position - off);
				splited_strings.push_back(stemp);
				off = find_position;
				off += spliter.length();
				find_position = orign.find(spliter, off);
			}
			if (off < orign.length())
			{
				stemp = orign.substr(off, orign.length() - off);
				splited_strings.push_back(stemp);
			}
			return splited_strings.size();
		}
	}
}

#endif