#pragma once
#include "define.h"

BEGIN_APP_NAMESPACE

#define VIEW_PROVIDER_NAME  _T("IViewProviderName")
#define VIEW_SERVICE_NAME   _T("IViewProvider")

class IViewSystem
{
public:
	virtual void RunUI() = 0;
};
END_APP_NAMESPACE