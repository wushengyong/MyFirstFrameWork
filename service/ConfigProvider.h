#pragma once

#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"
#include "interface/IConfigService.h"
#include "interface/ILogSystem.h"
#include "Json/json.h"


BEGIN_APP_NAMESPACE

class ConfigService
	: public IGlobalConfigService
	, public ILocalConfigService
{
public:
	ConfigService(const tstring& strFilePath = _T(""));
	
	virtual Json::Value      GetConfigNode(const tstring& strNodeName) override;
	virtual bool             Serialize(const Json::Value& config) override;
	virtual Json::Value      Deserialize() override;
	
private:
	Json::Value m_jsonRoot;
	bool        m_bIsOpened;
	tstring     m_strFilePath;
};


class ConfigProvider
	: public IServiceProvider
	, public IServiceConsumer
{
public:
	ConfigProvider();
	virtual bool         GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring      GetProviderName() const override;
	
	virtual void         ConsumeService(IServiceProvider* pServiceProvider) override;

	tstring              GetApplicationDir();
private:
	ConfigService        m_globalConfig;
	std::map<tstring, ConfigService> m_mapLocalConfigs;
};

END_APP_NAMESPACE