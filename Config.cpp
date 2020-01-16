#include "Config.h"
#include <cpprest/json.h>
#include <filesystem>

Config::Config()
{
	std::filesystem::path	curPath = std::filesystem::current_path();
	curPath.append(L"BingIMG");
	m_strBingWallpaperLocalPath = curPath.wstring();

	if (!std::filesystem::exists(m_strBingWallpaperLocalPath))
	{
		std::filesystem::create_directory(m_strBingWallpaperLocalPath);
	}

	Reload();
}

Config::~Config()
{
}

void Config::Reload()
{
	std::wfstream	wfs(L"config.json");
	web::json::value configJson = web::json::value::parse(wfs);
	if (configJson.is_object()
		&& configJson.has_string_field(L"BingCNURL")
		&& configJson.has_string_field(L"BingCNImgURL")
		&& configJson.at(L"BingCNURL").is_string()
		&& configJson.at(L"BingCNImgURL").is_string())
	{
		m_strBingURL = configJson.at(L"BingCNURL").as_string();
		m_strBingImgInfoURL = configJson.at(L"BingCNImgURL").as_string();
	}
}
