
#include "stlinc.h"
#include <Windows.h>
#include <strsafe.h>
#include <iterator>
namespace Common
{
    namespace util
    {
        t_string FormatErrorMessage()
        {
            LPVOID lpMsgBuf;
            DWORD error = GetLastError();
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                NULL, error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);
            t_string buffer;
            buffer.resize(256);
            StringCchPrintf(&buffer[0], 256, TEXT("%s"), (LPTSTR)lpMsgBuf);
            buffer.shrink_to_fit();
            LocalFree(lpMsgBuf);

            return buffer;
        }

        t_string NormallizeSearchPath(const t_string& path)
        {
            t_string searchpath;
            searchpath.resize(path.length());
            std::replace_copy(path.begin(), path.end(), searchpath.begin(), TEXT('/'), TEXT('\\'));

            if (searchpath[searchpath.length()-1] != TEXT('\\') )
            {
                searchpath += TEXT('\\');
            }

            return searchpath;
        }

    }
}