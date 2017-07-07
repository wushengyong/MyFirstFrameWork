#include "stdafx.h"
#include "App.h"
#include "service/ServiceProvidersManager.h"
#include "interface/IViewProvider.h"

App::App() : m_pViewProvider(NULLs)
{
}


App::~App()
{
}

void App::Init()
{
	m_serviceManager = std::tr1::shared_ptr<ServiceProvidersManager>(new ServiceProvidersManager());
	m_serviceManager->InitServices();
	m_serviceManager->AddServiceConsumer(this);
}

void App::Run()
{
	m_pViewProvider->RunUI();
}
void App::ConsumeService(IServiceProvider* pServiceProvider)
{
	pServiceProvider->GetService(_T("APP"), VIEW_PROVIDER_NAME, VIEW_SERVICE_NAME, (void**)&m_pViewProvider);
}
