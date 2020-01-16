#pragma once
#include <string>
class IConfig
{
public:
	static IConfig* Get();
	virtual void Reload() = 0;
	virtual std::wstring GetBingURL() = 0;
	virtual std::wstring GetBingImgInfoURL() = 0;
	virtual std::wstring GetBingWallpaperLocalPath() = 0;
protected:
private:
};