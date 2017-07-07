#include "stdafx.h"
#include "ConfigProvider.h"
#include <fstream>

BEGIN_APP_NAMESPACE

#define GLOBAL_CONFIG_PATH _T("config.json")

ConfigService::ConfigService(const tstring& strFileName)
{
	std::ifstream ifstream(strFileName);
	Json::Reader reader;
	if (!reader.parse(ifstream,configRoot)){
		m_pLogProvider->Log(DEBUG_LOG, strFileName + _T("Cannot be parsed."));
	}
}

Json::Value GetConfigNode(const tstring& strNodeName)
{
	return configRoot[strNodeName];
}

ConfigProvider::ConfigProvider() : m_globalConfig(GLOBAL_CONFIG_PATH)
{
	
}
bool ConfigProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (CONFIG_RPOVIDER_NAME == strServiceProvider) {
		if (GLOBAL_CONFIG_SERVICE_NAME == strServiceName){
			*pService = (void*)&m_globalConfig;
			return true;
		}
		else if (LOCAL_CONFIG_SERVICE_NAME == strServiceName){
			return false;
		}

	}
	return false;
}

tstring ConfigProvider::GetProviderName() const
{
	return CONFIG_RPOVIDER_NAME;
}
void ConfigProvider::ConsumeService(IServiceProvider* pServiceProvider)
{
	pServiceProvider->GetService(GetProviderName(),LOG_PROVIDE_NAME, LOG_SERVICE_NAME, (void**)&m_globalConfig.m_pLogProvider);
}

END_APP_NAMESPACE