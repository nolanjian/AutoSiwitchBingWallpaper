#include <cassert>
#include <filesystem>
#include "CBingSDKHelper.h"
#include "IConfig.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

CBingSDKHelper::CBingSDKHelper()
{
}

CBingSDKHelper::~CBingSDKHelper()
{
}

std::wstring CBingSDKHelper::GetTodayWallpaper()
{
	if (RequestImgInfoJson() && RequestImgFile())
	{
		return strLocalPath;
	}
	return std::wstring();
}

bool CBingSDKHelper::RequestImgInfoJson()
{
	IConfig* pConfig = IConfig::Get();
	assert(pConfig);

	web::http::client::http_client client(pConfig->GetBingImgInfoURL());

	std::function<void(web::http::http_response response)>	fnDual = std::bind(&CBingSDKHelper::ReadJsonBody, this, std::placeholders::_1);

	pplx::task<web::http::http_response> resp = client.request(web::http::methods::GET);
	resp.then(fnDual).wait();
	return !strURL.empty() && !strHsh.empty();
}

void CBingSDKHelper::ReadJsonBody(web::http::http_response response)
{
	web::http::http_headers& headers = response.headers();
	if (!headers.has(U("Content-Type")) || headers[U("Content-Type")].find(L"json") == std::wstring::npos)
	{
		return;
	}

	web::json::value jv = response.extract_json(true).get();
	if (jv.is_object() && jv.has_array_field(U("images")))
	{
		web::json::array& arr = jv.at(U("images")).as_array();
		web::json::value& jvIMG = arr.at(0);

		if (jvIMG.is_object() && jvIMG.has_string_field(U("url")))
		{
			web::json::value& urlObj = jvIMG.at(U("url"));
			strURL = urlObj.as_string();
		}
		std::wstring hshString;
		if (jvIMG.is_object() && jvIMG.has_string_field(U("hsh")))
		{
			web::json::value& hshObj = jvIMG.at(U("hsh"));
			strHsh = hshObj.as_string();

			std::filesystem::path imgFolder(IConfig::Get()->GetBingWallpaperLocalPath());
			imgFolder.append(strHsh + L".jpg");
			strLocalPath = imgFolder.wstring();
		}
	}
}

bool CBingSDKHelper::RequestImgFile()
{
	IConfig* pConfig = IConfig::Get();
	assert(pConfig);

	web::http::client::http_client client(pConfig->GetBingURL() + strURL);
	pplx::task<web::http::http_response> resp = client.request(web::http::methods::GET);
	web::http::http_response response = resp.get();
	web::http::http_headers& headers = response.headers();
	if (!headers.has(U("Content-Type")) || headers[U("Content-Type")].find(L"jpeg") == std::wstring::npos)
	{
		return false;
	}

	pplx::task<concurrency::streams::basic_ostream<uint8_t>> outStreamTask
		= concurrency::streams::fstream::open_ostream(strLocalPath, std::ios::out | std::ios::trunc);
	outStreamTask.wait();
	concurrency::streams::basic_ostream<uint8_t> outStream = outStreamTask.get();
	pplx::task<size_t> readTask = response.body().read_to_end(outStream.streambuf());
	readTask.wait();

	size_t fileSize = readTask.get();
	if (!std::filesystem::exists(strLocalPath) && fileSize > 0)
	{
		return false;
	}

	return true;
}
