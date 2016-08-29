#pragma once
#include "IAction.h"
#include <iosfwd>
namespace Common
{
    class Logger:public IAction
    {
    public:
        Logger(std::basic_ostream<TCHAR>& output,const std::shared_ptr<IAction>& decorated);
        ~Logger();

        // Í¨¹ý IAction ¼Ì³Ð
        virtual bool Act(const Archive & waste_file) override;

    private:
        std::shared_ptr<IAction> decorated_;
        std::basic_ostream<TCHAR>& output_;
        double logger_size_;
    };


}