#include "stdafx.h"
#include "stlinc.h"
#include "MoveAction.h"
#include <Windows.h>
#include <ShlObj.h>
namespace Common
{	
	DWORD Callback(LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID lpData)
	{
		MoveAction* host = static_cast<MoveAction*>(lpData);
		if (host)
		{			
			return host->ProgressCallback(TotalBytesTransferred.QuadPart, TotalFileSize.QuadPart) ? PROGRESS_CONTINUE : PROGRESS_CANCEL;
		}

		return PROGRESS_QUIET;
	}

	std::wstring ParseDir(const std::wstring& path)
	{
		auto result = path.rfind(L'\\');
		if (result != std::wstring::npos)
		{		
			return path.substr(0, result);
		}
		return L"";
	}

	MoveAction::MoveAction(const std::wstring & destDirectory)
		:dest_path_(destDirectory), orignal_reserved_(true)
	{
		dest_dir_ = ParseDir(destDirectory);
	}
	void MoveAction::ReserveOrignal(bool reserved)
	{
		orignal_reserved_ = reserved;
	}

	void MoveAction::Path(const std::wstring & path)
	{
		dest_path_ = path;

		dest_dir_ = ParseDir(path);
	}

	bool MoveAction::Act(const Archive & archive)
	{
		if (!dest_dir_.empty())
		{
			int create_dir = SHCreateDirectory(NULL, dest_dir_.c_str());
			if (create_dir != ERROR_SUCCESS)
			{
				std::wstring error = util::FormatErrorMessage();
				return false;
			}
		}
		int ret = CopyFileEx(archive.FullName().c_str(), dest_path_.c_str(), (LPPROGRESS_ROUTINE)Callback, this, NULL, 0);
		if (!ret)
		{			
			std::wstring errormessage = util::FormatErrorMessage();
			return false;
		}

		if (!orignal_reserved_)
		{
			DeleteFile(archive.FullName().c_str());
		}

		return true;
	}
	//only can move between the same volumn
	/*
	bool MoveAction::Act(const Archive & archive)
	{
		auto func = [&](LARGE_INTEGER TotalFileSize,
			LARGE_INTEGER TotalBytesTransferred,
			LARGE_INTEGER StreamSize,
			LARGE_INTEGER StreamBytesTransferred,
			DWORD dwStreamNumber,
			DWORD dwCallbackReason,
			HANDLE hSourceFile,
			HANDLE hDestinationFile,
			LPVOID lpData) -> unsigned long
		{

			return this->ProgressCallback(TotalBytesTransferred.QuadPart, TotalFileSize.QuadPart) ? PROGRESS_CONTINUE : PROGRESS_CANCEL;
		};

		DWORD dwFlags = MOVEFILE_WRITE_THROUGH;

		if (!orignal_reserved_)
		{
			dwFlags |= MOVEFILE_COPY_ALLOWED;
		}
		
		int ret = MoveFileWithProgress(archive.FullName().c_str(), dest_directory_.c_str(),
			(LPPROGRESS_ROUTINE)&func, this, dwFlags);

		if (!ret)
		{
			std::wstring errormessage = util::FormatErrorMessage();
			return false;
		}
		return true;
	}
	//*/
}