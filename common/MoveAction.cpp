#include "stdafx.h"
#include "stlinc.h"
#include "MoveAction.h"
#include <Windows.h>
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

	MoveAction::MoveAction(const std::wstring & destDirectory)
		:dest_directory_(destDirectory), orignal_reserved_(true)
	{

	}
	void MoveAction::ReserveOrignal(bool reserved)
	{
		orignal_reserved_ = reserved;
	}

	bool MoveAction::Act(const Archive & archive)
	{
		int ret = CopyFileEx(archive.FullName().c_str(), dest_directory_.c_str(), (LPPROGRESS_ROUTINE)Callback, this, NULL, 0);
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