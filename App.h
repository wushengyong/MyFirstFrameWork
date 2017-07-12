#pragma once

#include "interface/IServiceConsumer.h"
#include "interface/IServiceProvider.h"
#include "interface/IApp.h"
#include <memory>

BEGIN_APP_NAMESPACE



class ServiceProvidersManager;
class IViewSystem;

class App
	: public IServiceConsumer
	, public IServiceProvider
	, public IAppInfo
{
public:
	App();
	~App();

	void         Init(HINSTANCE hInst);
	void         Run();




	virtual void          ConsumeService(IServiceProvider* pServiceProvider) override;

	virtual bool          GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService);
	virtual tstring       GetProviderName() const { return APP_PROVIDER_NAME; }

	virtual HINSTANCE GetAppInstance() const { return m_hInst; }

private:
	std::tr1::shared_ptr<ServiceProvidersManager>  m_serviceManager;
	IViewSystem*                                   m_pViewSystem;
	HINSTANCE                                      m_hInst;
};

END_APP_NAMESPACE