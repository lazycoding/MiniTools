#pragma once
#include "Archive.h"
namespace Common {
    class IFilter {
    public:
        virtual ~IFilter() = default;
        IFilter() = default;
        virtual bool Match(const Archive&) = 0;

    };
}