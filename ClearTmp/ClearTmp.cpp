// ClearTmp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Clearner.h"
#include "ExtFilter.h"
#include "EraseAction.h"
#include "Logger.h"
#include <iostream>
using namespace std;
using namespace ClearTmp;


#ifndef RUN_TEST
unique_ptr<Cleanner> gcleanner;
int main(int argc, char** argv)
{
    shared_ptr<IFilter> filter(new ExtFilter{ ".clw", ".plg", ".ncb", ".opt" });

    shared_ptr<IAction> action = make_shared<EraseAction>();

    shared_ptr<IAction> wrapper = make_shared<Logger>(cout, action);

    gcleanner = make_unique<Cleanner>(filter);

    gcleanner->Clean(R"(d:\Code\depot\Dbas\BugFix\DBas60_Development\BroadCast\Projects.tmp\Debug\)", wrapper);

    return 0;
}
#endif


#ifdef RUN_TEST
#include "../CppUnitLite_stl/TestHarness.h"
#include "Scanner.h"

int main()
{
    TestResult tr;
    TestRegistry::runAllTests(tr);
    system("pause");
    return 0;
}

TEST(WasteFile, ctor)
{
    WasteFile wf(TEXT("D:\\1.txt"), 10);
    CHECK(wf.Name().compare(TEXT("1.txt")) == 0);
    CHECK(wf.Ext().compare(TEXT(".txt")) == 0);
}


TEST(ExtFilter, ctor)
{
    ExtFilter ef{ TEXT(".clw"), TEXT(".plg"), TEXT(".ncb"), TEXT(".opt") };
    auto bl = ef.Blacklist();
    for each (auto var in bl)
    {
        wcout << var << TEXT(" ");
    }
    cout << endl;
    CHECK_LONGS_EQUAL(4, bl.size());
}

TEST(ExtFilter, func)
{
    ExtFilter ef{ TEXT(".clw"), TEXT(".plg"), TEXT(".ncb"), TEXT(".opt") };
    WasteFile wf;
    wf.FullName(TEXT("D:\\abc.cpp"));
    
    CHECK(!ef.Match(wf));

    wf.FullName(TEXT("D:\\eee.clw"));
    CHECK(ef.Match(wf));
}

TEST(EraseAction, func)
{
    shared_ptr<IAction> act = make_shared<EraseAction>();
    WasteFile wf(TEXT("D:\\1.txt"), 10);
    try
    {
        bool erased = act->Act(wf);
        CHECK(erased);
    }
    catch (const std::exception& except)
    {
        cerr << except.what() << endl;
        static const bool exception_occur = false;
        CHECK(exception_occur);
    }
}

TEST(Scanner, func)
{
    Scanner ascanner;
    vector<WasteFile> files;
    ascanner.Wildcards(TEXT("*.*"));
    ascanner.Traverse(TEXT("d:\\Code\\Projects\\ClearTmp\\"), files);
    
    /*setlocale(LC_ALL, "zh-CN");*/
    std::locale loc("");
    std::wcout.imbue(loc);
    for each (auto file in files)
    {
        wcout << file.FullName() << endl;
    }
}
#endif // RUN_TEST

