#include "stdafx.h"
#include "ExtFilter.h"
#include <iterator>
namespace ClearTmp
{

    ExtFilter::ExtFilter(std::initializer_list<std::string> bl)
    {
        CopyStringVecUpper(bl, black_list_);
    }

    void ExtFilter::Blacklist(const std::vector<std::string>& black_list)
    {
        CopyStringVecUpper(black_list, black_list_);
    }

    const std::vector<std::string>& ExtFilter::Blacklist() const
    {
        return black_list_;
    }

    bool ExtFilter::Match(const WasteFile & wf)
    {
        if (black_list_.empty())
        {
            return false;
        }

        auto found = std::find_if(black_list_.begin(), black_list_.end(), [&](const std::string& ext)->bool {
            std::string ext_upper = wf.Ext();
            std::transform(ext_upper.begin(), ext_upper.end(), ext_upper.begin(), toupper);
            return ext.compare(ext_upper) == 0;
        });

        return found != black_list_.end();
    }

}
