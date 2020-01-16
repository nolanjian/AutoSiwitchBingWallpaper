#include "CBingSDKHelper.h"

IBingSDKHelper* IBingSDKHelper::Get()
{
	static CBingSDKHelper sCBingSDKHelper;
	return &sCBingSDKHelper;
}
