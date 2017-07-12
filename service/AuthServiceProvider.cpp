#include "stdafx.h"
#include "AuthServiceProvider.h"

BEGIN_APP_NAMESPACE
bool AuthProvider::IsConsumerCanUseService(const tstring& strConsumerName, const tstring& strProviderName, const tstring& strServiceName) const
{
	return true; // 当前所有的权限均正常使用
}

bool AuthProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (AUTH_RPOVIDER_NAME == strServiceProvider && AUTH_SERVICE_NAME == strServiceName) {
		*pService = (void*)this;
		return true;
	}
	return false;
}

tstring AuthProvider::GetProviderName() const
{
	return AUTH_RPOVIDER_NAME;
}


void AuthProvider::ConsumeService(IServiceProvider* pServiceProvider)
{

}

END_APP_NAMESPACE