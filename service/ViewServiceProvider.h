#pragma once

#include "interface/IViewProvider.h"

BEGIN_APP_NAMESPACE

class ViewProvider : public IViewProvider
{
public:
	virtual void RunUI() override;

	virtual bool GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring GetProviderName() const override;

};

END_APP_NAMESPACE