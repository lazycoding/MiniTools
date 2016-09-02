#include "stdafx.h"
#include "ExifReader.h"
#include <fstream>
#include <exception>
using namespace std;

namespace Common
{
#define  TIMELINE_BEGIN 13
#define  BASE 16
#define  MAX_STRLEN 100
	ExifReader::ExifReader()
	{
	}

	bool ExifReader::Open(const wchar_t * file)
	{
		ifstream ifs(file, ifstream::binary);
		if (!ifs.good())
		{
			return false;
		}

		string exif_str = exif::extract_exif(ifs);
		if (exif_str.empty())
		{	
			return false;
		}
		return exif_info_.parse(exif_str);
	}

	const char* ExifReader::DateTimeOriginal() const
	{
		auto& original = exif_info_.DateTimeOriginal;		
		return original.c_str();;
	}


}