#pragma once
#include "define.h"
#include "IServiceProvider.h"

BEGIN_APP_NAMESPACE

#define VIEW_PROVIDER_NAME  _T("IViewProviderName")
#define VIEW_SERVICE_NAME   _T("IViewProvider")

class IViewProvider : public IServiceProvider
{
public:
	virtual void RunUI() = 0;
};
END_APP_NAMESPACE