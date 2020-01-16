#pragma once
#include "IConfig.h"
class Config : public IConfig
{
public:
	Config();
	~Config();
	virtual void Reload() override;
	virtual std::wstring GetBingURL() override { return m_strBingURL; }
	virtual std::wstring GetBingImgInfoURL() override { return m_strBingImgInfoURL; }
	virtual std::wstring GetBingWallpaperLocalPath() override { return m_strBingWallpaperLocalPath; }
protected:
	
private:
	std::wstring m_strBingURL;
	std::wstring m_strBingImgInfoURL;
	std::wstring m_strBingWallpaperLocalPath;
};