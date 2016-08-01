// ClearTmp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Clearner.h"
#include "ExtFilter.h"
#include "EraseAction.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <codecvt>
#include <sstream>
using namespace std;
using namespace ClearTmp;

int LoadUtf8File(const t_string& file_name, vector<t_string>& suffixs)
{
    wifstream ifs;
    auto utf8 = locale(locale(""), new codecvt_utf8<wchar_t>());
    ifs.imbue(utf8);
    ifs.open(file_name, ios::binary);
    if (!ifs.good())
    {
        return 0;
    }

    size_t size = (size_t)ifs.seekg(0,ios::end).tellg();
    t_string content;
    content.resize(size);
    ifs.seekg(0, ios::beg);
    ifs.read(&content[0], size);

    replace(content.begin(), content.end(), TEXT('\r'), TEXT('\n'));

    wstringstream wss(content);

    t_string suffix;
    while (getline(wss, suffix))
    {
        if (!suffix.empty())
        {
            suffixs.push_back(suffix);
        }        
    }

    ifs.close();
    return 1;
}

//#undef RUN_TEST
#ifndef RUN_TEST
unique_ptr<Cleanner> gcleanner;

void Usage();

int main(int argc, TCHAR** argv)
{
    if (argc != 3)
    {
        Usage();
        return 1;
    }
    
    vector<t_string> suffixs;
    if (!LoadUtf8File(argv[3], suffixs))
    {
        return 2;
    }

    shared_ptr<IFilter> filter = make_shared<ExtFilter>(suffixs);

    shared_ptr<IAction> action = make_shared<EraseAction>();

    std::locale loc("");
    std::wcout.imbue(loc);
    shared_ptr<IAction> wrapper = make_shared<Logger>(wcout, action);

    gcleanner = make_unique<Cleanner>(filter);

    gcleanner->Clean(argv[2], wrapper);

    system("pause");

    return 0;
}

#endif


#ifdef RUN_TEST
#include "../CppUnitLite_stl/TestHarness.h"
#include "Scanner.h"
#include "ClearTmp.h"

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

TEST(LoadUtf8File, func)
{
    vector<t_string> suff;
    LoadUtf8File(TEXT("D:\\2.txt"), suff);
    CHECK(suff.size()>0);
}
#endif // RUN_TEST

