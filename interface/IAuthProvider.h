#pragma once
#include "define.h"
#include "IServiceProvider.h"

#define AUTH_RPOVIDER_NAME   _T("AuthProvider")
#define AUTH_SERVICE_NAME    _T("AuthService")

BEGIN_APP_NAMESPACE

class IAuthProvider : public IServiceProvider
{
public:
	virtual bool IsConsumerCanUseService(const tstring& strConsumerName, const tstring& strProviderName, const tstring& strServiceName) const = 0;
};

END_APP_NAMESPACE