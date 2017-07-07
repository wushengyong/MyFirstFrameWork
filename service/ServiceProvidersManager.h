#pragma once

#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"
#include <map>
#include <vector>
#include <memory>

class IAuthProvider;
class ILogProvider;
class IViewProvider;

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

	std::tr1::shared_ptr<IAuthProvider>   m_auth;
	std::tr1::shared_ptr<ILogProvider>    m_log;
	std::tr1::shared_ptr<IViewProvider>   m_view;
};

