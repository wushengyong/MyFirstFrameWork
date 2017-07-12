#include "stdafx.h"
#include "ConfigProvider.h"
#include <fstream>
#include "base/getexedir.h"
#include <ShlObj.h>
#include "json/json.h"
#include <fstream>

BEGIN_APP_NAMESPACE

#define GLOBAL_CONFIG_PATH _T("\\EplateBarcodeRW\\config.json")

ConfigService::ConfigService(const tstring& strFilePath) 
	: m_strFilePath(strFilePath)
	, m_bIsOpened(false)
{

}


Json::Value ConfigService::GetConfigNode(const tstring& strNodeName)
{
	if (!m_bIsOpened){
		Deserialize();
	}
	return m_jsonRoot[strNodeName];
}

bool ConfigService::Serialize(const Json::Value& config)
{
	std::ofstream ofstream(m_strFilePath);
	Json::StyledStreamWriter writer;
	writer.write(ofstream, config);
	return true;
}
Json::Value ConfigService::Deserialize()
{
	std::ifstream ifstream(m_strFilePath);
	Json::Reader reader;
	reader.parse(ifstream, m_jsonRoot);
	return m_jsonRoot;
}
ConfigProvider::ConfigProvider() : m_globalConfig(GetExeDir() + GLOBAL_CONFIG_PATH)
{
	
}
bool ConfigProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (CONFIG_RPOVIDER_NAME == strServiceProvider) {
		if (GLOBAL_CONFIG_SERVICE == strServiceName){
			*pService = dynamic_cast<IGlobalConfigService*>(&m_globalConfig);
			return true;
		}
		else if (LOCAL_CONFIG_SERVICE == strServiceName){
			tstring strPath  = GetApplicationDir() + _T("\\") +strConsumer;
			if (m_mapLocalConfigs.find(strPath) == m_mapLocalConfigs.end()){
				m_mapLocalConfigs[strPath] = ConfigService(strPath);
			}
			*pService = dynamic_cast<ILocalConfigService*>(&m_mapLocalConfigs[strPath]);
			return true;
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
}

tstring app::ConfigProvider::GetApplicationDir()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, true);
	return szPath;
}



END_APP_NAMESPACE

