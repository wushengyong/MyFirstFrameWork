#include "stdafx.h"
#include "ViewServiceProvider.h"
#include "DuiLib/UIlib.h"
#include "interface/IApp.h"
#include "ui/LoginUI.h"
#include "ui/MainUI.h"

BEGIN_APP_NAMESPACE


ViewProvider::ViewProvider()
	: m_pLogSystem(NULL)
	, m_pNetService(NULL)
	, m_pDevService(NULL)
	, m_pLocalConfigService(NULL)
{

}


void ViewProvider::RunUI()
{
	UINT nRet = WM_MAINVIEW_SWITCH_USER;
	while (nRet == WM_MAINVIEW_SWITCH_USER){
		LoginUI* pLogin = new LoginUI();

		pLogin->Init(m_pNetService, m_pLocalConfigService, m_pLogSystem);
		
		pLogin->CenterWindow();
		nRet = pLogin->ShowModal();


		if (nRet == IDOK){
			MainUI* pMainUI = new MainUI();
			pMainUI->Init(m_pLogSystem, m_pDevService, m_pNetService);
			pMainUI->CenterWindow();
			nRet = pMainUI->ShowModal();

		}
	}
}

bool ViewProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (VIEW_PROVIDER_NAME == strServiceProvider && VIEW_SERVICE_NAME == strServiceName) {
		*pService = (void*)this;
		return true;
	}
	return false;
}

tstring ViewProvider::GetProviderName() const
{
	return VIEW_PROVIDER_NAME;
}

void ViewProvider::ConsumeService(IServiceProvider* pServiceProvider)
{
	if (pServiceProvider){

		IAppInfo* pAppInfo = nullptr;
		if (pServiceProvider->GetService(VIEW_PROVIDER_NAME, APP_PROVIDER_NAME, APP_SERVICE_NAME, (void**)&pAppInfo)
			&& pAppInfo != nullptr){
			CPaintManagerUI::SetInstance(pAppInfo->GetAppInstance());
		}

		pServiceProvider->GetService(VIEW_PROVIDER_NAME, LOG_PROVIDE_NAME, LOG_SERVICE_NAME, (void**)&m_pLogSystem);
		pServiceProvider->GetService(VIEW_PROVIDER_NAME, DEV_PROVIDER_NAME, DEV_SERVICE_NAME, (void**)&m_pDevService);
		pServiceProvider->GetService(VIEW_PROVIDER_NAME, NET_PROVIDER_NAME, NET_SERVICE_NAME, (void**)&m_pNetService);
		pServiceProvider->GetService(LOGIN_UI_CONSUMER, CONFIG_RPOVIDER_NAME, LOCAL_CONFIG_SERVICE, (void**)&m_pLocalConfigService);
	}
}

END_APP_NAMESPACE

