#pragma once
#include "interface/ILogger.h"
#include <fstream>

class FileLogger : public ILogger
{
public:
	FileLogger(const tstring& strFilePath, const LOG_TYPE& minLogType = DEBUG_LOG);
	virtual ~FileLogger();

	virtual void Log(LOG_TYPE logType, const tstring& strLog) override;

private:
	tstring             m_strFilePath;
	LOG_TYPE            m_nMinLogType;
	std::ofstream       m_stream;
};

