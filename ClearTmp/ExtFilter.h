#pragma once
#include "IFilter.h"
#include  <initializer_list>
namespace ClearTmp {
    class ExtFilter : public IFilter 
    {
    public:
        ExtFilter() = default;
        ~ExtFilter() = default;
        ExtFilter(std::initializer_list<std::string> bl);

        ExtFilter(const ExtFilter&) = delete;
        ExtFilter operator=(const ExtFilter&) = delete;

        void Blacklist(const std::vector<std::string>& black_list);
        const std::vector<std::string>& Blacklist() const;

        // Í¨¹ý IFilter ¼Ì³Ð
        virtual bool Match(const WasteFile &) override;

    private:
        std::vector<std::string> black_list_;        
    };

    template<class T, class U>
    void CopyStringVecUpper(const T& in, U& out)
    {
        std::transform(std::begin(in), std::end(in), std::back_inserter(out),
            [&](const std::string& str)->std::string {
            std::string ret;
            std::transform(str.begin(), str.end(), std::back_inserter(ret), ::toupper);
            return ret;
        });
    }
}