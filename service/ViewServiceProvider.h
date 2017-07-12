#pragma once

#include "interface/IServiceProvider.h"
#include "interface/IViewSystem.h"
#include "interface/IServiceConsumer.h"
#include "interface/ILogSystem.h"
#include "interface/IDevService.h"
#include "interface/INetService.h"
#include "interface/IConfigService.h"

BEGIN_APP_NAMESPACE

class ViewProvider 
	: public IViewSystem
	, public IServiceProvider
	, public IServiceConsumer
{
public:
	ViewProvider();
	virtual void            RunUI() override;

	virtual bool            GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring         GetProviderName() const override;

	virtual void            ConsumeService(IServiceProvider* pServiceProvider) override;

private:
	ILogSystem*             m_pLogSystem;
	IDevService*            m_pDevService;
	INetService*            m_pNetService;
	ILocalConfigService*    m_pLocalConfigService;
};

END_APP_NAMESPACE