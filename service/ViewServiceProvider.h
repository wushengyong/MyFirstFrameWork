#pragma once

#include "interface/IViewProvider.h"

class ViewProvider : public IViewProvider
{
public:
	virtual void RunUI() override;

	virtual bool GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring GetProviderName() const override;

};