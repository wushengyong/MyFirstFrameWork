#pragma once

#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"
#include <map>
#include <vector>
#include <memory>

BEGIN_APP_NAMESPACE

class AuthProvider;
class LogProvider;
class ViewProvider;
class ConfigProvider;
class NetProvider;
class DevProvider;


#define PROVIDER_NAME  _T("ServiceProvidersManager")
#define SERVICE_NAME   _T("ServiceProvider")

typedef std::map<tstring, IServiceProvider*> MAP_SERVICE_PROVIDER;
typedef std::vector<IServiceConsumer*>       VEC_SERVICE_CONSUMER;

class ServiceProvidersManager 
	: public IServiceProvider
	, public IServiceConsumer
{
public:
	ServiceProvidersManager();
	virtual ~ServiceProvidersManager();

public:
	void                  InitServices();
public: // IServiceProvider
	virtual bool          GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;
	virtual tstring       GetProviderName() const override;
public: // IServiceConsumer
	virtual void          ConsumeService(IServiceProvider* pServiceProvider) override;
public:
	bool                  AddServiceProvider(IServiceProvider* pServiceProvider);
	void                  RemovServiceProvider(IServiceProvider* pServiceProvider);
	bool                  AddServiceConsumer(IServiceConsumer* pServiceConsumer);
	void                  RemovServiceConsumer(IServiceConsumer* pServiceConsumer);
private:
	MAP_SERVICE_PROVIDER                  m_mapServiceProviders;
	VEC_SERVICE_CONSUMER                  m_vecServiceConsumer;

	std::tr1::shared_ptr<AuthProvider>           m_auth;
	std::tr1::shared_ptr<LogProvider>            m_log;
	std::tr1::shared_ptr<ViewProvider>           m_view;
	std::tr1::shared_ptr<ConfigProvider>         m_config;
	std::tr1::shared_ptr<NetProvider>            m_netService;
	std::tr1::shared_ptr<DevProvider>            m_devService;
};

END_APP_NAMESPACE