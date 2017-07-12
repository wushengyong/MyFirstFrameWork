#include "stdafx.h"
#include "NetServiceProvider.h"
#include "interface/IConfigService.h"
#include "json/json.h"

BEGIN_APP_NAMESPACE


#define CLSID_NET_CMM ""
#define SERVER_NODE   _T("server")

NetProvider::NetProvider() : m_pLogSystem(NULL)
{

}


NetProvider::~NetProvider()
{
	Logout();
}

bool NetProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (NET_PROVIDER_NAME == strServiceProvider && NET_SERVICE_NAME == strServiceName) {
		*pService = (void*)this;
		return true;
	}
	return false;
}

tstring NetProvider::GetProviderName() const
{
	return NET_PROVIDER_NAME;
}



void NetProvider::ConsumeService(IServiceProvider* pServiceProvider)
{
	IGlobalConfigService* pGlobalConfig = nullptr;
	pServiceProvider->GetService(NET_PROVIDER_NAME, CONFIG_RPOVIDER_NAME, GLOBAL_CONFIG_SERVICE, (void**)&pGlobalConfig);
	if (pGlobalConfig){
		m_strServerUrl = pGlobalConfig->GetConfigNode(SERVER_NODE).asString();
	}

	pServiceProvider->GetService(NET_PROVIDER_NAME, LOG_PROVIDE_NAME, LOG_SERVICE_NAME, (void**)&m_pLogSystem);
}

bool NetProvider::Login(const tstring& strUserName, const tstring& strPasswd)
{
	INetComm* pNetCmm = GetNetCmm();
	if (pNetCmm == nullptr)return false;

	if (!m_strToken.empty()){
		Logout();
	}

	ServerResponse res = pNetCmm->Login(m_strServerUrl,strUserName, strPasswd);
	if (res.code != HTTP_STATUS_OK){
		Warning(_T("Login Failed."));
		Debug(_T("Server Return: ") + res.res + _T(""));
		return false;
	}

	Info(_T("Login Succeed."));

	Json::Reader reader;
	Json::Value  value;

	if (reader.parse(res.res, value)){
		m_strToken = value[_T("token")].asString();
	}
	return true;
}

bool NetProvider::Logout()
{
	if (!m_strToken.empty()){
		INetComm* pNetCmm = GetNetCmm();
		if (pNetCmm == nullptr)return false;
		ServerResponse res = pNetCmm->Logout(m_strServerUrl, m_strToken);
		if (res.code != HTTP_STATUS_OK){
			Warning(_T("Logout Failed."));
			Debug(_T("Server Return: ") + res.res + _T(""));
			return false;
		}
		Info(_T("Logout Succeed."));
		m_strToken = _T("");
	}

	return true;
}


bool NetProvider::CreateEplate(const tstring& strBarcode, const tstring& strEplateType)
{
	if (!m_strToken.empty()){
		INetComm* pNetComm = GetNetCmm();
		if (pNetComm == nullptr)return false;
		ServerResponse res = pNetComm->CreateEplate(m_strServerUrl, strBarcode, strEplateType, m_strToken);
		if (res.code != HTTP_STATUS_OK){
			Warning(_T("Cannot Create Eplate : ") + strBarcode + _T(" EplateType : ") + strEplateType);
			Debug(_T("Server Return : ") + res.res);
			return false;
		}
		Info(_T("Create Eplate ") + strBarcode + _T(" EplateType : ") + strEplateType + _T(" Succeed."));
		return true;
	}
	return false;
}

ezlife_net::INetComm* NetProvider::GetNetCmm()
{
	if (m_netCmm.p() == nullptr){
		m_netCmm = x3::Object<INetComm>(CLSID_NET_CMM);
	}
	return m_netCmm.p();
}



void NetProvider::Info(const tstring& strText)
{
	if (m_pLogSystem){
		m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, strText);
	}
}

void NetProvider::Error(const tstring& strText)
{
	if (m_pLogSystem){
		m_pLogSystem->Log(LOG_TYPE::ERROR_LOG, strText);
	}
}

void NetProvider::Warning(const tstring& strText)
{
	if (m_pLogSystem){
		m_pLogSystem->Log(LOG_TYPE::WARNING_LOG, strText);
	}
}

void NetProvider::Debug(const tstring& strText)
{
	if (m_pLogSystem){
		m_pLogSystem->Log(LOG_TYPE::DEBUG_LOG, strText);
	}
}

END_APP_NAMESPACE