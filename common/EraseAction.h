#pragma once
#include "IAction.h"

namespace Common {
    class EraseAction :public IAction
    {
        // Í¨¹ý IAction ¼Ì³Ð
        virtual bool Act(const Archive & waste_file) override;
    };
}