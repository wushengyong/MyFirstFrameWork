#pragma once

#include "interface/IAuthProvider.h"

class AuthProvider : public IAuthProvider
{
public:
	virtual bool IsConsumerCanUseService(const tstring& strConsumerName, const tstring& strProviderName, const tstring& strServiceName) const override;

	virtual bool GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring GetProviderName() const override;

};