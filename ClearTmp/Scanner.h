#pragma once
#include "stdafx.h"
#include "WasteFile.h"
namespace ClearTmp 
{  
    class IFilter;
    class Scanner
    {
    public:
        Scanner() :wildcards_(TEXT("*.*")) {}

        Scanner(std::shared_ptr<IFilter>& ptr)
            :filter_(ptr), wildcards_(TEXT("*.*"))
        {

        }
        ~Scanner()=default;

        Scanner(const Scanner& other) = delete;
        const Scanner& operator=(const Scanner& rhs) = delete;
        
        int Traverse(const t_string & path, std::vector<Archive>& waste_files);

        void Wildcards(const t_string& wildcards);
    private:
        std::shared_ptr<IFilter> filter_;
        t_string wildcards_;
    };
}