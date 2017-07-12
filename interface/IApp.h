#pragma once

#include "define.h"
#include <windows.h>

#define APP_PROVIDER_NAME  _T("AppProvider")
#define APP_SERVICE_NAME   _T("AppService")

BEGIN_APP_NAMESPACE

class IAppInfo
{
public:
	virtual HINSTANCE GetAppInstance() const = 0;
};

END_APP_NAMESPACE