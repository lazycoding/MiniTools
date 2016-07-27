#include "stdafx.h"
#include "Clearner.h"
#include "Scanner.h"
namespace ClearTmp 
{
    Cleanner::Cleanner()
    {

    }

    Cleanner::Cleanner(std::shared_ptr<IFilter>& filter)
        :scanner_(std::make_shared<Scanner>(filter))
    {
    }
    bool Cleanner::Clean(const t_string& path, const std::shared_ptr<IAction>& action)
    {
        return false;
    }
}