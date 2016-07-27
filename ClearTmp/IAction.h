#pragma once
#include "WasteFile.h"
namespace ClearTmp
{
    class IAction
    {
    public:
        virtual ~IAction() = default;
        virtual bool Act(const WasteFile& waste_file) = 0;
    };
}