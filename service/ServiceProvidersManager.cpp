#include "stdafx.h"
#include "ServiceProvidersManager.h"
#include "AuthServiceProvider.h"
#include "LogServiceProvide.h"
#include "ViewServiceProvider.h"
#include "ConfigProvider.h"
#include "NetServiceProvider.h"
#include "DevServiceProvider.h"

BEGIN_APP_NAMESPACE

ServiceProvidersManager::ServiceProvidersManager()
{
}


ServiceProvidersManager::~ServiceProvidersManager()
{
}

void ServiceProvidersManager::InitServices()
{
	// 可以读取DLL中的IServiceProvider跟IServiceConsumer或者自己搞
	m_auth       = std::tr1::shared_ptr<AuthProvider>(new AuthProvider());
	m_log        = std::tr1::shared_ptr<LogProvider>(new LogProvider());
	m_view       = std::tr1::shared_ptr<ViewProvider>(new ViewProvider());
	m_config     = std::tr1::shared_ptr<ConfigProvider>(new ConfigProvider());
	m_netService = std::tr1::shared_ptr<NetProvider>(new NetProvider());
	m_devService = std::tr1::shared_ptr<DevProvider>(new DevProvider());


	AddServiceProvider(m_auth.get());
	AddServiceProvider(m_log.get());
	AddServiceProvider(m_config.get());
	AddServiceProvider(m_view.get());
	AddServiceProvider(m_netService.get());
	AddServiceProvider(m_devService.get());

	AddServiceConsumer(m_auth.get());
	AddServiceConsumer(m_log.get());
	AddServiceConsumer(m_config.get());
	AddServiceConsumer(m_view.get());
	AddServiceConsumer(m_netService.get());
	AddServiceConsumer(m_devService.get());

}

bool ServiceProvidersManager::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (!m_auth->IsConsumerCanUseService(strConsumer, strServiceProvider, strServiceName))return false;

	if (strServiceProvider == GetProviderName() && strServiceName == SERVICE_NAME) {
		*pService = (void*)this;
		return true;
	}
	else {
		auto itr = m_mapServiceProviders.find(strServiceProvider);
		if (itr != m_mapServiceProviders.end()) {
			itr->second->GetService(strConsumer, strServiceProvider, strServiceName, pService);
			return true;
		}
	}
	return false;
}

tstring ServiceProvidersManager::GetProviderName() const
{
	return PROVIDER_NAME;
}

void ServiceProvidersManager::ConsumeService(IServiceProvider* pServiceProvider)
{
	for (auto itr = m_vecServiceConsumer.begin(); itr != m_vecServiceConsumer.end(); ++itr) {
		(*itr)->ConsumeService(this);
	}
}

bool ServiceProvidersManager::AddServiceProvider(IServiceProvider* pServiceProvider)
{
	if (m_mapServiceProviders.find(pServiceProvider->GetProviderName()) != m_mapServiceProviders.end()) {
		return false;
	}
	m_mapServiceProviders[pServiceProvider->GetProviderName()] = pServiceProvider;
	return true;
}

void ServiceProvidersManager::RemovServiceProvider(IServiceProvider* pServiceProvider)
{
	auto itr = m_mapServiceProviders.find(pServiceProvider->GetProviderName());
	if (itr != m_mapServiceProviders.end()) {
		m_mapServiceProviders.erase(itr);
	}
}

bool ServiceProvidersManager::AddServiceConsumer(IServiceConsumer* pServiceConsumer)
{
	m_vecServiceConsumer.push_back(pServiceConsumer);
	return true;
}

void ServiceProvidersManager::RemovServiceConsumer(IServiceConsumer* pServiceConsumer)
{
	auto itr = std::find(m_vecServiceConsumer.begin(), m_vecServiceConsumer.end(), pServiceConsumer);
	if (itr != m_vecServiceConsumer.end()){
		m_vecServiceConsumer.erase(itr);
	}
}

END_APP_NAMESPACE