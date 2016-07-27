#include "stdafx.h"
#include "EraseAction.h"

namespace ClearTmp {
  
    bool EraseAction::Act(const WasteFile & waste_file)
    {
        bool deleted = false;
        try
        {
            deleted = DeleteFile(waste_file.FullName().c_str()) != 0;
            if (!deleted)
            {
                throw std::runtime_error("");
            }
        }
        catch (...)
        {
            t_string error_message = util::FormatErrorMessage();
            t_string log = TEXT("Erase file ");
            log += waste_file.FullName();
            log += TEXT(" :");
            log += error_message;

            std::string mbslog;
            mbslog.resize(log.length() * 2);
            size_t converted;
            wcstombs_s(&converted, &mbslog[0], mbslog.length(), log.c_str(), _TRUNCATE);
            mbslog.shrink_to_fit();
            throw std::runtime_error(mbslog);
        }
        return deleted;
    }    
}
