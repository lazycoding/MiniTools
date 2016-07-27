#include "stdafx.h"
#include "Logger.h"

namespace ClearTmp {

    Logger::Logger(std::ostream& output, std::shared_ptr<IAction>& decorated)
        :decorated_(decorated), output_(output)
    {
    }


    Logger::~Logger()
    {
    }

    bool Logger::Act(const WasteFile & waste_file)
    {
        output_ << "prepare to action "
            << waste_file.FullName() << " "
            << waste_file.Ext() << " "
            << waste_file.Size() << std::endl;
        bool result = false;
        try
        {
            if (decorated_)
            {
                result = decorated_->Act(waste_file);
            }
        }
        catch (const std::exception& except)
        {
            output_ << "exception:" << except.what() << std::endl;
        }
       
        output_ << "completed " << (result ? "successfully" : "unsuccessfully") << std::endl;

        return result;
    }

}