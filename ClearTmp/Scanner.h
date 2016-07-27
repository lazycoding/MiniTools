#pragma once
#include "stdafx.h"
#include "WasteFile.h"
namespace ClearTmp 
{  
    class IFilter;
    class Scanner
    {
    public:
        Scanner(std::shared_ptr<IFilter>& ptr):filter_(ptr)
        {

        }
        ~Scanner()=default;

        Scanner(const Scanner& other) = delete;
        const Scanner& operator=(const Scanner& rhs) = delete;

        int Traverse(const std::string& path, std::vector<WasteFile>& waste_files);

    private:
        std::shared_ptr<IFilter> filter_;
    };
}