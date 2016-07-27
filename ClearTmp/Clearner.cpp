#include "stdafx.h"
#include "Clearner.h"
#include "Scanner.h"
#include "IAction.h"
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
        if (scanner_)
        {
            scanner_->Traverse(path, waste_files_);
        }

        if (action)
        {
            for each (auto file in waste_files_)
            {
                action->Act(file);
            }
        }

        return true;
    }
}