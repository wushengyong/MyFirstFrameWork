#pragma once

#include "interface/IAuth.h"
#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"

BEGIN_APP_NAMESPACE

class AuthProvider 
	: public IAuth
	, public IServiceProvider
	, public IServiceConsumer
{
public:
	virtual bool IsConsumerCanUseService(const tstring& strConsumerName, const tstring& strProviderName, const tstring& strServiceName) const override;

public:
	virtual bool          GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;
	virtual tstring       GetProviderName() const override;

	virtual void          ConsumeService(IServiceProvider* pServiceProvider) override;

};
END_APP_NAMESPACE