#include "stdafx.h"
#include "EraseAction.h"
#include <windows.h>
#include <strsafe.h>
std::string FormatErrorMessage();

bool ClearTmp::EraseAction::Act(const WasteFile & waste_file)
{
    bool deleted = false;
    try
    {
        std::wstring file_name;
        file_name.resize(waste_file.FullName().length() + 1);
        size_t convertedSize;
        auto ret = mbstowcs_s(&convertedSize, &file_name[0], file_name.length(), waste_file.FullName().c_str(), waste_file.FullName().length());
        if (ret != -1)
        {
            deleted = DeleteFile(file_name.c_str()) != 0;
        }
        if (!deleted)
        {
            throw std::runtime_error(waste_file.FullName());
        }
    }    
    catch (...)
    {
        std::string error_message = FormatErrorMessage();
        std::string log = "Erase file ";
        log += waste_file.FullName();
        log += " :";
        log += error_message;
        throw std::runtime_error(log);
    }
    return deleted;
}


std::string FormatErrorMessage()
{
    LPVOID lpMsgBuf;
    DWORD error = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    std::wstring buffer;
    buffer.resize(256);
    StringCchPrintf(&buffer[0], 256, TEXT("%s"), (LPTSTR)lpMsgBuf);
    buffer.shrink_to_fit();

    std::string error_message;
    error_message.resize(buffer.length()*2);
    setlocale(LC_ALL, "zh-CN");
    size_t convertedSize;
    wcstombs_s(&convertedSize, &error_message[0], error_message.length(), buffer.c_str(), buffer.length());
        
    return error_message;
}
