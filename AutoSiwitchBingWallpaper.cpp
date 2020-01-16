#include <windows.h>
#include <assert.h>
#include <thread>
#include <iostream>
#include "IBingSDKHelper.h"
#include "ActiveDesktopHelper.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define SERVICE_NAME L"AutoSiwitchBingWallpaper"
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;

static bool s_bShouldStop = false;
std::thread* pMyThreadTask = nullptr;

void DoTask()
{
	bool bChange = false;
	while (!bChange)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		IBingSDKHelper* pSDK = IBingSDKHelper::Get();
		assert(pSDK);
		bChange = ActiveDesktopHelper::SetDesktop(pSDK->GetTodayWallpaper());
	}
}

void WINAPI ServiceHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;

		if (pMyThreadTask && pMyThreadTask->joinable())
		{
			s_bShouldStop = true;
			pMyThreadTask->join();

			delete pMyThreadTask;
			pMyThreadTask = nullptr;
		}

		break;
	default:
		return;
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}
}

void WINAPI service_main(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceHandler);
	if (hServiceStatusHandle == 0)
	{
		DWORD nError = GetLastError();
	}
	//add your init code here

	//add your service thread here
	pMyThreadTask = new std::thread(DoTask);

	// Initialization complete - report running status 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 9000;
	if (FALSE == SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}

}

int main()
{
	//SERVICE_TABLE_ENTRY ServiceTable[2];

	//ServiceTable[0].lpServiceName = const_cast<wchar_t*>(SERVICE_NAME);
	//ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

	//ServiceTable[1].lpServiceName = NULL;
	//ServiceTable[1].lpServiceProc = NULL;
	//// 启动服务的控制分派机线程
	//StartServiceCtrlDispatcher(ServiceTable);
	//return 0;
	DoTask();
	return 0;
}
