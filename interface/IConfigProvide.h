#pragma once
#include "define.h"
#include "IServiceProvider.h"

#define CONFIG_RPOVIDER_NAME             _T("ConfigProvider")
#define GLOBAL_CONFIG_SERVICE_NAME       _T("GlobalConfigServiceName")
#define LOCAL_CONFIG_SERVICE_NAME        _T("LocalConfigServiceName")

BEGIN_APP_NAMESPACE

class IConfigProvider : public IServiceProvider
{
};

class IGlobalConfig
{
public:
};

class ILocalConfig
{
public:
};

END_APP_NAMESPACE