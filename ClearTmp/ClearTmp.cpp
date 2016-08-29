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
using namespace Common;


int LoadUtf8File(const t_string& file_name, vector<t_string>& suffixs)
{
    wifstream ifs;
    auto utf8 = locale(locale(""), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>());
    ifs.imbue(utf8);
    ifs.open(file_name, ios::binary);
    if (!ifs.good())
    {
        return 0;
    }

    size_t size = (size_t)ifs.seekg(0, ios::end).tellg();
    wstring content;
    content.resize(size);
    ifs.seekg(0, ios::beg);
    ifs.read(&content[0], size);

    replace(content.begin(), content.end(), TEXT('\r'), TEXT('\n'));

    wstringstream wss(content);

    wstring suffix;
    while (getline(wss, suffix))
    {
        if (!suffix.empty())
        {
#ifdef _UNICODE
            suffixs.push_back(suffix);
#else
            size_t converted;
            setlocale(LC_ALL, "zh-CN");
            string str;
            str.resize(suffix.size() * 2);
            auto ret = wcstombs_s(&converted, &str[0], str.length(), suffix.c_str(), _TRUNCATE);
            suffixs.push_back(str);
#endif // _UNICODE

        }
    }

    ifs.close();
    return 1;
}

//#undef RUN_TEST

#ifndef RUN_TEST
unique_ptr<Cleanner> gcleanner;

void Usage();

vector<t_string> default_suffixs{ TEXT(".clw"), TEXT(".plg"), TEXT(".ncb"), TEXT(".opt"), TEXT(".aps") };

int _tmain(int argc, TCHAR** argv)
{
    shared_ptr<IFilter> filter;
    shared_ptr<IAction> action;

    if (argc == 3)
    {
        vector<t_string> suffixs;
        t_string opt = argv[2];
        if (_tcsicmp(argv[2], TEXT("/d")) == 0)
        {
            suffixs = default_suffixs;
        }
        else if (!LoadUtf8File(argv[3], suffixs))
        {
            return 2;
        }

        filter = make_shared<ExtFilter>(suffixs);
        auto inner_action = make_shared<EraseAction>();

        std::locale loc("");
        std::wcout.imbue(loc);
        action = make_shared<Logger>(wcout, inner_action);
    }
    else if (argc == 2)
    {

    }
    else
    {
        Usage();
        return 1;
    }

    gcleanner = make_unique<Cleanner>(filter);

    gcleanner->Clean(argv[1], action);
    
    return 0;
}

void Usage()
{
    std::locale loc("");
    std::wcout.imbue(loc);
    wcout << L"Usage:\tcleartmp target_path [filter file]" << endl;
    wcout << L"      \tFilter file is needed to be encoded by utf8." << endl;
}
#endif


#ifdef RUN_TEST
#include "../CppUnitLite/TestHarness.h"
#include "Scanner.h"
#include "ClearTmp.h"

int main()
{
    TestResult tr;
    TestRegistry::runAllTests(tr);
    system("pause");
    return 0;
}

TEST(Archive, ctor)
{
    Archive wf(TEXT("D:\\1.txt"), 10);
    CHECK(wf.Name().compare(TEXT("1.txt")) == 0);
    CHECK(wf.Ext().compare(TEXT(".txt")) == 0);
}


TEST(ExtFilter, ctor)
{    
    vector<t_string> list{ TEXT(".clw"), TEXT(".plg"), TEXT(".ncb"), TEXT(".opt") };
    ExtFilter ef(list);
    auto bl = ef.Blacklist();
    /*
    for each (auto var in bl)
    {
        wcout << var << TEXT(" ");
    }
    cout << endl;
    */
    CHECK_LONGS_EQUAL(4, bl.size());
}

TEST(ExtFilter, func)
{
    vector<t_string> list{ TEXT(".clw"), TEXT(".plg"), TEXT(".ncb"), TEXT(".opt") };
    ExtFilter ef(list);
    Archive wf;
    wf.FullName(TEXT("D:\\abc.cpp"));

    CHECK(!ef.Match(wf));

    wf.FullName(TEXT("D:\\eee.clw"));
    CHECK(ef.Match(wf));
}

TEST(EraseAction, func)
{
    shared_ptr<IAction> act = make_shared<EraseAction>();
    Archive wf(TEXT("D:\\1.txt"), 10);
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
    vector<Archive> files;
    ascanner.Wildcards(TEXT("*.*"));
    ascanner.Traverse(TEXT("d:\\Code\\Projects\\ClearTmp\\"), files);

    /*setlocale(LC_ALL, "zh-CN");*/
    /*
    std::locale loc("");
    std::wcout.imbue(loc);
    for each (auto file in files)
    {
        wcout << file.FullName() << endl;
    }
    */
}

TEST(LoadUtf8File, func)
{
    vector<t_string> suff;
    LoadUtf8File(TEXT("D:\\2.txt"), suff);
    CHECK(suff.size() > 0);
}
#endif // RUN_TEST

