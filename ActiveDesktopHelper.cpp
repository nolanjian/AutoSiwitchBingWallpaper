#include "ActiveDesktopHelper.h"
#include <windows.h>
#include <WinInet.h>
#include <ShlObj.h>
#include <ShObjIdl_core.h>

bool ActiveDesktopHelper::SetDesktop(std::wstring strImgPath)
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return false;
	}

	IActiveDesktop* pActiveDesktop = NULL;
	hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&pActiveDesktop);
	if (FAILED(hr) || !pActiveDesktop)
	{
		return false;
	}

	hr = pActiveDesktop->SetWallpaper(strImgPath.c_str(), 0);
	if (FAILED(hr))
	{
		return false;
	}
	hr = pActiveDesktop->ApplyChanges(AD_APPLY_ALL);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
