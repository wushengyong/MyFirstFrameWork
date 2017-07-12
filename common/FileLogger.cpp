#include "stdafx.h"
#include "FileLogger.h"
#include "DateTimeTools.h"

BEGIN_APP_NAMESPACE
FileLogger::FileLogger(const tstring& strFilePath, const LOG_TYPE& minLogType /*= DEBUG_LOG*/)
{
	m_strFilePath = strFilePath;
	m_nMinLogType = minLogType;
	m_stream.open(strFilePath, std::ios::app);
}


FileLogger::~FileLogger()
{
}

void FileLogger::Log(LOG_TYPE logType, const tstring& strLog)
{
	if (logType >= m_nMinLogType) {
		if (m_stream.is_open()) {
			m_stream << GetCurrentDateTimeStr() + _T(" ") + strLog << std::endl;
		}
	}
}
END_APP_NAMESPACE