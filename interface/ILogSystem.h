#pragma once
#include "define.h"
#include "IServiceProvider.h"
#include "ILogger.h"

BEGIN_APP_NAMESPACE

#define LOG_PROVIDE_NAME      _T("LogServerProvider")
#define LOG_SERVICE_NAME     _T("LogService")


class ILogSystem : public ILogger
{
public:
	virtual void     AddLogger(ILogger* pLogger) = 0;
	virtual void     RemoveLogger(ILogger* pLogger) = 0;
};

END_APP_NAMESPACE