#pragma once
#include "IAction.h"

namespace ClearTmp {
    class EraseAction :public IAction
    {
        // Í¨¹ý IAction ¼Ì³Ð
        virtual bool Act(const WasteFile & waste_file) override;
    };
}