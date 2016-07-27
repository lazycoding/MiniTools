#include "stdafx.h"
#pragma once
namespace ClearTmp
{
    class WasteFile
    {
    public:
        WasteFile() :size_(0) {}

        WasteFile(const std::string& full_name, long size)
            :full_name_(full_name), size_(size)
        {
            ParseNameAndExt();
        }
        ~WasteFile() = default;

        const std::string& FullName() const
        {
            return full_name_;
        }

        void FullName(const std::string& full_name)
        {
            full_name_ = full_name;
            ParseNameAndExt();
        }

        long Size() const
        {
            return size_;
        }

        void Size(long size)
        {
            size_ = size;
        }

        const std::string& Name() const
        {         
            return name_;
        }

        const std::string& Ext() const
        {         
            return ext_;
        }

    private:
        void ParseNameAndExt()
        {
            if (full_name_.empty())
            {
                return;
            }            
            auto result = full_name_.rfind('\\');
            if (result != std::string::npos)
            {
                name_ = full_name_.substr(result+1);
                auto found = name_.find_first_of('.');
                if (found != std::string::npos)
                {
                    ext_ = name_.substr(found);
                }
            }
        }


    private:
        std::string full_name_;
        std::string name_;
        std::string ext_;
        long size_;
    };

}
