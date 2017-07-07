#pragma once
#include "define.h"

BEGIN_APP_NAMESPACE

enum LOG_TYPE
{
	DEBUG_LOG,
	WARNING_LOG,
	NORMAL_LOG,
	ERROR_LOG,
};


class ILogger
{
public:
	virtual void Log(LOG_TYPE logType, const tstring& strLog) = 0;
};

END_APP_NAMESPACE