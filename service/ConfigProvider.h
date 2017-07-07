#pragma once

#include "interface/IConfigProvider.h"
#include "interface/IConfigService.h"
#include "interface/ILogProvider.h"

BEGIN_APP_NAMESPACE

class ConfigService : public IConfigService
{
public:
	ConfigService(const tstring& strFileName);
	
	virtual Json::Value GetConfigNode(const tstring& strNodeName) override;
	
private:
	Json::Value configRoot;
	
public:
	ILogProvider* m_pLogProvider;
};

class ConfigProvider 
	: public IConfigProvider
	, public IServiceConsumer
{
public:
	ConfigProvider();
	virtual bool         GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring      GetProviderName() const override;
	
	virtual void         ConsumeService(IServiceProvider* pServiceProvider) override;

private:
	ConfigService        m_globalConfig;
};

END_APP_NAMESPACE