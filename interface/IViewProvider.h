#pragma once
#include "define.h"
#include "IServiceProvider.h"

#define VIEW_PROVIDER_NAME  _T("IViewProviderName")
#define VIEW_SERVICE_NAME   _T("IViewProvider")

class IViewProvider : public IServiceProvider
{
public:
	virtual void RunUI() = 0;
};