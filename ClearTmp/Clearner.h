#pragma once
#include "WasteFile.h"


namespace ClearTmp
{
    class Scanner;
    class IFilter;
    class IAction;
    class Cleanner
    {
    public:
        Cleanner();
        Cleanner(std::shared_ptr<IFilter>& filter);
        ~Cleanner() = default;
        Cleanner(const Cleanner&) = delete;
        Cleanner operator=(const Cleanner&) = delete;

        bool Clean(const std::string& path, const std::shared_ptr<IAction>& action);

    private:
        std::vector<WasteFile> waste_files_;
        std::shared_ptr<Scanner> scanner_;        
    };
}
