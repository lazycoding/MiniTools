#pragma once
#include "IAction.h"
#include <string>
#include <functional>
namespace Common
{

	class MoveAction:public IAction
	{
	public:
		MoveAction(const std::wstring& destDirectory = L"");
				
		std::function<bool(long long transferd, long long total)> ProgressCallback;

		void ReserveOrignal(bool reserved);

		void Path(const std::wstring& path);

		// Í¨¹ý IAction ¼Ì³Ð
		virtual bool Act(const Archive & archive) override;		
	private:
		std::wstring dest_path_;
		std::wstring dest_dir_;
		bool orignal_reserved_;
		
		friend DWORD Callback(LARGE_INTEGER TotalFileSize,
			LARGE_INTEGER TotalBytesTransferred,
			LARGE_INTEGER StreamSize,
			LARGE_INTEGER StreamBytesTransferred,
			DWORD dwStreamNumber,
			DWORD dwCallbackReason,
			HANDLE hSourceFile,
			HANDLE hDestinationFile,
			LPVOID lpData);
	};


}