#include "stdafx.h"
#include "Scanner.h"
#include "IFilter.h"
using namespace std;
namespace ClearTmp 
{
    
    int Scanner::Traverse(const t_string & path, std::vector<Archive>& waste_files)
    {
        t_string searchPath = util::NormallizeSearchPath(path);
        WIN32_FIND_DATA finddata;
        HANDLE hfind = FindFirstFile(t_string(searchPath + wildcards_).c_str(), &finddata);
        while (hfind!=INVALID_HANDLE_VALUE)
        {
            if (finddata.cFileName[0] == TEXT('.'))
            {
                // . and .. is directory
                //continue;
            }
            else if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                t_string subsearch = searchPath + finddata.cFileName;
                Traverse(subsearch, waste_files);                
            }
            else
            {
                LARGE_INTEGER filesize;
                filesize.LowPart = finddata.nFileSizeLow;
                filesize.HighPart = finddata.nFileSizeHigh;

                Archive file;
                file.FullName(searchPath + finddata.cFileName);
                file.Size(filesize.QuadPart);

                if (!filter_ || filter_->Match(file))
                {
                    waste_files.push_back(file);
                }                
            }                    

            BOOL next = FindNextFile(hfind, &finddata);
            if (!next)
            {
                break;
            }
        }

        FindClose(hfind);

        return 0;
    }

    void Scanner::Wildcards(const t_string & wildcards)
    {
        wildcards_ = wildcards;
    }

}