#pragma once
#include <string>
class IBingSDKHelper
{
public:
	static IBingSDKHelper* Get();
	virtual std::wstring GetTodayWallpaper() = 0;
protected:
private:
};