#pragma once
#include "define.h"

#define DEV_PROVIDER_NAME    _T("DevProviderName")
#define DEV_SERVICE_NAME     _T("DevServiceName")

BEGIN_APP_NAMESPACE

class IDevService
{
public:
	virtual bool     ConnectDevice() = 0;
	virtual bool     DisconnectDevice() = 0;
	virtual bool     WriteBarcode(const tstring& strBarcode) = 0;
	virtual tstring  ReadBarcode() = 0;
	virtual tstring  GetEplateType() const = 0;
};

END_APP_NAMESPACE