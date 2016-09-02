// PhotosTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../CppUnitLite/TestHarness.h"
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
	MoveAction act(L"D:\\video.exe");
	act.ProgressCallback = [&](long long transfered, long long total)->bool
	{
		auto str=lzns::str::format("moving...%I64d/%I64d\t%.2lf%%", transfered, total, (double)transfered/total * 100);
		cout << str << endl;
		return true;
	};

	Archive ar;
	ar.FullName(L"C:\\Users\\liuzhongyuan\\Downloads\\Video_Driver_RX8XH_WN32_16.200.2001_A02.EXE");
	bool ret = act.Act(ar);
	CHECK(ret);
}

int main()
{
	UnitTest();
	return 0;
}



