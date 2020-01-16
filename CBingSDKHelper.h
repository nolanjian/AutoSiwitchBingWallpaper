#pragma once
#include "IBingSDKHelper.h"
#include <cpprest/http_client.h>


class CBingSDKHelper : public IBingSDKHelper
{
public:
	CBingSDKHelper();
	~CBingSDKHelper();
	virtual std::wstring GetTodayWallpaper() override;
protected:
	bool RequestImgInfoJson();
	void ReadJsonBody(web::http::http_response response);
	bool RequestImgFile();
private:
	std::wstring strURL;
	std::wstring strHsh;
	std::wstring strLocalPath;
};