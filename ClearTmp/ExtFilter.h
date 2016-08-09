#pragma once
#include "IFilter.h"
#include  <initializer_list>
namespace ClearTmp {
    class ExtFilter : public IFilter 
    {
    public:
        ExtFilter() = default;
        ~ExtFilter() = default;
        ExtFilter(const std::vector<t_string>& black_list);

        ExtFilter(const ExtFilter&) = delete;
        ExtFilter operator=(const ExtFilter&) = delete;

        const std::vector<t_string>& Blacklist() const;

        // Í¨¹ý IFilter ¼Ì³Ð
        virtual bool Match(const Archive &) override;

    private:
        std::vector<t_string> black_list_;        
    };

    template<class T, class U>
    void CopyStringVecUpper(const T& in, U& out)
    {
        std::transform(std::begin(in), std::end(in), std::back_inserter(out),
            [&](const t_string& str)->t_string {
            t_string ret;
            std::transform(str.begin(), str.end(), std::back_inserter(ret), ::_totupper);
            return ret;
        });
    }
}