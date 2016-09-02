#pragma once
#include "Archive.h"
namespace Common
{
    class IAction
    {
    public:
        virtual ~IAction() = default;
        virtual bool Act(const Archive& archive) = 0;
    };
}