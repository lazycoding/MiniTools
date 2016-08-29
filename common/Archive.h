#pragma once
namespace Common
{
    class Archive
    {
    public:
        Archive() :size_(0) {}

        Archive(const t_string& full_name, __int64 size)
            :full_name_(full_name), size_(size)
        {
            ParseNameAndExt();
        }
        ~Archive() = default;

        const t_string& FullName() const
        {
            return full_name_;
        }

        void FullName(const t_string& full_name)
        {
            full_name_ = full_name;
            ParseNameAndExt();
        }

        __int64 Size() const
        {
            return size_;
        }

        void Size(__int64 size)
        {
            size_ = size;
        }

        const t_string& Name() const
        {         
            return name_;
        }

        const t_string& Ext() const
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
            auto result = full_name_.rfind(TEXT('\\'));
            if (result != t_string::npos)
            {
                name_ = full_name_.substr(result+1);
                auto found = name_.find_first_of(TEXT('.'));
                if (found != t_string::npos)
                {
                    ext_ = name_.substr(found);
                }
            }
        }


    private:
        t_string full_name_;
        t_string name_;
        t_string ext_;
        __int64 size_;
    };

}
