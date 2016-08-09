#pragma once
#include "stdafx.h"
#include "WasteFile.h"
namespace ClearTmp {
    class IFilter {
    public:
        virtual ~IFilter() = default;
        IFilter() = default;
        virtual bool Match(const Archive&) = 0;

    };
}