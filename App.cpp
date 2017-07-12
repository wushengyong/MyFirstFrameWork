#include "stdafx.h"
#include "App.h"
#include <Shlwapi.h>
#include "service/ServiceProvidersManager.h"
#include "interface/IViewSystem.h"
#include "interface/IConfigService.h"
#include "json/json.h"
#include "base/getexedir.h"
#include "x3py/interface/core/portability/x3port.h"
#include "x3py/interface/core/nonplugin/scanplugins.h"

BEGIN_APP_NAMESPACE

#define APP_NAME   _T("APP_NAME")

#define MODULE_NODE    _T("modules")
#define MODULE_NAME    _T("name")
#define MODULE_CLSID   _T("clsid")
#define MODULE_PATH    _T("path")

App::App() : m_pViewSystem(NULL), m_hInst(NULL)
{
}


App::~App()
{
}

void App::Init(HINSTANCE hInst)
{
	m_hInst = hInst;
	m_serviceManager = std::tr1::shared_ptr<ServiceProvidersManager>(new ServiceProvidersManager());
	m_serviceManager->InitServices();
	m_serviceManager->AddServiceConsumer(this);
	m_serviceManager->AddServiceProvider(this);
	m_serviceManager->ConsumeService(NULL);
}

void App::Run()
{
	m_pViewSystem->RunUI();
}
void App::ConsumeService(IServiceProvider* pServiceProvider)
{
	pServiceProvider->GetService(APP_NAME, VIEW_PROVIDER_NAME, VIEW_SERVICE_NAME, (void**)&m_pViewSystem);
	
	IGlobalConfigService* pGlobalConfig = nullptr;
	pServiceProvider->GetService(APP_NAME, CONFIG_RPOVIDER_NAME, GLOBAL_CONFIG_SERVICE, (void**)&pGlobalConfig);

	if (pGlobalConfig == nullptr)return;

	// 加载所有的modules
	Json::Value& jsonModules =  pGlobalConfig->GetConfigNode(MODULE_NODE);

	tstring strExePath = GetExeDir();
	// 先加载x3manager.pln

	tstring strx3Manager = strExePath + _T("x3manager.pln");
	x3::loadfilter(strx3Manager.c_str(), ::PathFindExtension(strx3Manager.c_str()));

	for (size_t i = 0; i < jsonModules.size(); ++i){
		Json::Value& module = jsonModules[i];

		tstring strPlnName = module[MODULE_NAME].asString();
		tstring strPlnClsid = module[MODULE_CLSID].asString();
		tstring strPlnPath = module[MODULE_PATH].asString();

		tstring strModulePath = strExePath + strPlnPath;
		x3::loadfilter(strModulePath.c_str(), ::PathFindExtension(strModulePath.c_str()));
	}
}


bool App::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (APP_PROVIDER_NAME == strServiceProvider && strServiceName == APP_SERVICE_NAME){
		*pService = dynamic_cast<IAppInfo*>(this); 
		return true;
	}
	return false;
}


END_APP_NAMESPACE