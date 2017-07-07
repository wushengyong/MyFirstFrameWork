#pragma once
#include "define.h"

class IServiceProvider
{
public:
	virtual bool          GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) = 0;
	virtual tstring       GetProviderName() const = 0;
};
