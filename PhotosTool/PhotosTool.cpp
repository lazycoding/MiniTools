// PhotosTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../CppUnitLite/TestHarness.h"
#include "Scanner.h"
#include "ExtFilter.h"
#include "ExifReader.h"
#include "MoveAction.h"
#include "strformat.h"
#include <iostream>
using namespace std;
using namespace Common;

int UnitTest()
{
	TestResult tr;
	TestRegistry::runAllTests(tr);
	system("pause");
	return 0;
}

std::wstring DateTimeToFileName(const char* strTime);

TEST(ExifReader, func)
{
	ExifReader er;
	bool ret = er.Open(L"D:\\Shared\\照片\\DSC00804.JPG");
	CHECK(ret == true);

	std::string str = er.DateTimeOriginal();
	CHECK(!str.empty());
}

TEST(MoveAction, func)
{
	MoveAction act(L"D:\\test\\1\\2\\video.exe");
	act.ProgressCallback = [&](long long transfered, long long total)->bool
	{
		auto str = lzns::str::format("moving...%I64d/%I64d\t%.2lf%%", transfered, total, (double)transfered / total * 100);
		cout << str << endl;
		return true;
	};

	Archive ar;
	ar.FullName(L"C:\\Users\\liuzhongyuan\\Downloads\\Video_Driver_RX8XH_WN32_16.200.2001_A02.EXE");
	bool ret = act.Act(ar);
	CHECK(ret);

	if (ret)
	{
		SHFILEOPSTRUCT op;
		op.wFunc = FO_DELETE;
		op.hwnd = NULL;
		op.pFrom = L"D:\\test";
		op.pTo = NULL;
		op.fFlags = FOF_NO_UI | FOF_NOERRORUI | FOF_SILENT;
		SHFileOperation(&op);
	}

}

TEST(DateTimeToFileName, func)
{
	auto str = DateTimeToFileName("2012:12:22 19:40:02");

	CHECK(str == L"\\2012\\1222_194002");
}

std::wstring DateTimeToFileName(const char* strTime)
{
	std::string ret;
	if (NULL != strTime)
	{
		int year, month, day, hour, minute, second;
		sscanf(strTime, "%d:%d:%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
		ret = lzns::str::format("\\%d\\%02d%02d_%d%d%d", year, month, day, hour, minute, second);
	}

	std::wstring FileName;
	FileName.reserve(ret.size() * 2);

	mbstowcs(&FileName[0], ret.c_str(), ret.size() * 2);

	return FileName;
}

int main()
{
	//UnitTest();
	wcout.imbue(locale(""));

	vector<std::wstring> suffix = { L".jpg" };
	wstring source_dir = L"";
	wstring dest_dir = L"";
	shared_ptr<IFilter> filter = make_shared<ExtFilter>(suffix);
	auto action = make_shared<MoveAction>();
	action->ReserveOrignal(true);
	action->ProgressCallback = [&](long long transfered, long long total)->bool
	{
		auto str = lzns::str::format(L"\t%I64d/%I64d bytes\t%.2lf%%", transfered, total, (double)transfered / total * 100);
		wcout << str << endl;
		return true;
	};

	Scanner sc(filter);
	vector<Archive> archs;
	sc.Traverse(source_dir, archs);

	ExifReader er;
	
	for (size_t i = 0; i < archs.size(); i++)
	{
		er.Open(archs[i].FullName().c_str());

		wstring name = DateTimeToFileName(er.DateTimeOriginal());
		wstring path = lzns::str::format(L"%s%s%s", dest_dir, name, archs[i].Ext());

		wcout << lzns::str::format(L"%s moving to %s......", archs[i].FullName().c_str(), path.c_str()) << endl;

		action->Path(path);

		action->Act(archs[i]);
	}
	return 0;
}



