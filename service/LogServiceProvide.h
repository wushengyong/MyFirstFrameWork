#pragma once

#include "interface/ILogProvider.h"
#include <vector>

typedef std::vector<ILogger*>   VEC_LOGGER;

class LogProvider : public ILogProvider
{
public:
	virtual void AddLogger(ILogger* pLogger) override;

	virtual void RemoveLogger(ILogger* pLogger) override;

	virtual bool GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring GetProviderName() const override;

	virtual void Log(LOG_TYPE logType, const tstring& strLog) override;

private:
	VEC_LOGGER   m_vecLoggers;
};