#pragma once
#include "define.h"

#define NET_PROVIDER_NAME   _T("NetProvider")
#define NET_SERVICE_NAME    _T("NetService")

BEGIN_APP_NAMESPACE

class INetService
{
public:
	virtual bool               Login(const tstring& strUserName, const tstring& strPasswd) = 0;
	virtual bool               Logout() = 0;
	virtual bool               CreateEplate(const tstring& strBarcode, const tstring& strEplateType) = 0;
};

END_APP_NAMESPACE