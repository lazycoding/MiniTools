#include "stdafx.h"
#include "ExtFilter.h"
#include <iterator>
namespace ClearTmp
{

    ExtFilter::ExtFilter(const std::vector<t_string>& black_list)
    {
        CopyStringVecUpper(black_list, black_list_);
    }

    const std::vector<t_string>& ExtFilter::Blacklist() const
    {
        return black_list_;
    }

    bool ExtFilter::Match(const WasteFile & wf)
    {
        if (black_list_.empty())
        {
            return false;
        }

        auto found = std::find_if(black_list_.begin(), black_list_.end(), [&](const t_string& ext)->bool {
            t_string ext_upper = wf.Ext();
            std::transform(ext_upper.begin(), ext_upper.end(), ext_upper.begin(), _totupper);
            return ext.compare(ext_upper) == 0;
        });

        return found != black_list_.end();
    }

}
