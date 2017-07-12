#pragma once

#include "interface/IServiceProvider.h"
#include "interface/ILogSystem.h"
#include "interface/IServiceConsumer.h"
#include <vector>
#include "common/FileLogger.h"


BEGIN_APP_NAMESPACE
typedef std::vector<ILogger*>   VEC_LOGGER;

class LogProvider 
	: public ILogSystem
	, public IServiceProvider
	, public IServiceConsumer
{
public:
	LogProvider();
	virtual ~LogProvider();
	virtual void AddLogger(ILogger* pLogger) override;

	virtual void RemoveLogger(ILogger* pLogger) override;



	virtual void Log(LOG_TYPE logType, const tstring& strLog) override;


public:
	virtual bool GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;
	virtual tstring GetProviderName() const override;

	virtual void ConsumeService(IServiceProvider* pServiceProvider) override;


	tstring GetApplicationDir();
private:
	VEC_LOGGER   m_vecLoggers;
	FileLogger   m_FileLogger;
};
END_APP_NAMESPACE