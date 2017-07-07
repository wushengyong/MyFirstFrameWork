#pragma once

#include "interface/IServiceConsumer.h"
#include <memory>

class ServiceProvidersManager;
class IViewProvider;

class App : public IServiceConsumer
{
public:
	App();
	~App();

	void Init();
	void Run();
	virtual void ConsumeService(IServiceProvider* pServiceProvider) override;

private:
	std::tr1::shared_ptr<ServiceProvidersManager>  m_serviceManager;
	IViewProvider*                                 m_pViewProvider;
};

