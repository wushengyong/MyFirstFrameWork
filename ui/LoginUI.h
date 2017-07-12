#pragma once
#include "DuiLib/UIlib.h"
#include "interface/define.h"
#include "interface/ILogger.h"
#include "interface/ILogSystem.h"
#include "interface/INetService.h"
#include "interface/IConfigService.h"
#include "json/json.h"
#include "boost/thread.hpp"

using namespace DuiLib;

typedef struct USER_INFO
{
public:
	tstring strName;
	tstring strPasswd;
} USER_INFO;


#define LOGIN_UI_CONSUMER  _T("LoginUIConsumer")

BEGIN_APP_NAMESPACE

class LoginUI 
	: public WindowImplBase
	, public ILogger
{
public:
	LoginUI();
	~LoginUI();

	void    Init(INetService* pNetService, ILocalConfigService* pConfig, ILogSystem* pLogSystem);

	virtual CDuiString GetSkinFolder() override;

	virtual CDuiString GetSkinFile() override;

	virtual LPCTSTR GetWindowClassName(void) const override;

	virtual void OnFinalMessage(HWND hWnd) override;

	virtual void Notify(TNotifyUI& msg) override;

	virtual void Log(LOG_TYPE logType, const tstring& strText) override;

	virtual void InitWindow() override;

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	void EnsureControlStatus(bool bRelativeCtrlStatus, bool bTooltipsVisibled, const tstring& strTooltips);
	void Login(const tstring& strUserName, const tstring& strPasswd, bool bRemeber);
protected:
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;


	void    LoadUserToUI();
	void    SerializeUsers();

public:
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static  HWND             hwnd;
	static  HHOOK            hook;
private:
	ILogSystem*              m_pLogSystem;
	INetService*             m_pNetService;
	ILocalConfigService*     m_pLocalConfig;

	CEditUI*                 m_pUserName;
	CEditUI*                 m_pPasswd;
	CComboUI*                m_pComboUserName;
	std::vector<USER_INFO>   m_users;
	std::map<tstring, int>   m_mapUserIndex;

	boost::thread            m_loginThread;
};

END_APP_NAMESPACE