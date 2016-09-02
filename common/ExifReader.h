#pragma once
#include "Exif.h"
#include <string>
namespace Common
{
	class ExifReader
	{
	public:
		ExifReader();
		
		bool Open(const wchar_t* file);

		const char* DateTimeOriginal() const;

	private:
		exif::Info exif_info_;
	};

}