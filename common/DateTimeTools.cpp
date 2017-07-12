#include "stdafx.h"
#include "DateTimeTools.h"

BEGIN_APP_NAMESPACE

tstring GetCurrentDateTimeStr()
{
	SYSTEMTIME sys = { 0 };
	GetLocalTime(&sys);
	CDuiString szDate;
	szDate.Format(_T("%04d%02d%02d %02d:%02d:%02d.%03d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	return szDate.GetData();
}

END_APP_NAMESPACE