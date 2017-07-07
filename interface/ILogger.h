#pragma once
#include "define.h"

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