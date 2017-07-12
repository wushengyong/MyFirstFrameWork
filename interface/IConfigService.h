#pragma once

#include "define.h"

#define CONFIG_RPOVIDER_NAME             _T("ConfigProvider")
#define GLOBAL_CONFIG_SERVICE            _T("GlobalConfigService")
#define LOCAL_CONFIG_SERVICE             _T("LocalConfigService")

namespace Json
{
	class Value;
}



BEGIN_APP_NAMESPACE

class IGlobalConfigService
{
public:
	virtual Json::Value GetConfigNode(const tstring& strNodeName) = 0;
};
class ILocalConfigService
{
public:
	virtual bool Serialize(const Json::Value& config) = 0;
	virtual Json::Value Deserialize() = 0;
};
END_APP_NAMESPACE