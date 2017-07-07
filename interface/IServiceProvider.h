#pragma once
#include "define.h"

BEGIN_APP_NAMESPACE

class IServiceProvider
{
public:
	virtual bool          GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) = 0;
	virtual tstring       GetProviderName() const = 0;
};
END_APP_NAMESPACE