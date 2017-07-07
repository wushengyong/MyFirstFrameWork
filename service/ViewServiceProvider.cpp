#include "stdafx.h"
#include "ViewServiceProvider.h"

void ViewProvider::RunUI()
{
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
