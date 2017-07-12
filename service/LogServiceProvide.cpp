#include "stdafx.h"
#include "LogServiceProvide.h"
#include "Base/getexedir.h"
#include <ShlObj.h>

BEGIN_APP_NAMESPACE


LogProvider::LogProvider() : m_FileLogger(GetApplicationDir() + _T("\\Log.txt"))
{
	AddLogger(&m_FileLogger);
}

LogProvider::~LogProvider()
{
	m_vecLoggers.clear();
}

void LogProvider::AddLogger(ILogger* pLogger)
{
	if (std::find(m_vecLoggers.begin(), m_vecLoggers.end(), pLogger) == m_vecLoggers.end()) {
		m_vecLoggers.push_back(pLogger);
	}
}

void LogProvider::RemoveLogger(ILogger* pLogger)
{
	auto itr = std::find(m_vecLoggers.begin(), m_vecLoggers.end(), pLogger);
	if (itr != m_vecLoggers.end()) {
		m_vecLoggers.erase(itr);
	}
}

bool LogProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (strServiceProvider == LOG_PROVIDE_NAME && strServiceName == LOG_SERVICE_NAME) {
		*pService = (void*)this;
		return true;
	}
	return false;
}

tstring LogProvider::GetProviderName() const
{
	return LOG_PROVIDE_NAME;
}


void LogProvider::ConsumeService(IServiceProvider* pServiceProvider)
{

}


tstring app::LogProvider::GetApplicationDir()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, true);
	return szPath;
}

void LogProvider::Log(LOG_TYPE logType, const tstring& strLog)
{
	for (size_t i = 0; i < m_vecLoggers.size(); ++i) {
		m_vecLoggers[i]->Log(logType, strLog);
	}
}
END_APP_NAMESPACE