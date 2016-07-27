#pragma once
#include "IAction.h"
#include <iosfwd>
namespace ClearTmp
{
    class Logger:public IAction
    {
    public:
        Logger(std::basic_ostream<TCHAR>& output, std::shared_ptr<IAction>& decorated);
        ~Logger();

        // Í¨¹ý IAction ¼Ì³Ð
        virtual bool Act(const WasteFile & waste_file) override;

    private:
        std::shared_ptr<IAction>& decorated_;
        std::basic_ostream<TCHAR>& output_;
    };


}