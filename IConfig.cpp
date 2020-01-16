#include "Config.h"

IConfig* IConfig::Get()
{
	static Config sConfig;
	return &sConfig;
}
