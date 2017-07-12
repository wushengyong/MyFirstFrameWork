#include "stdafx.h"
#include "LoginUI.h"
#include "project_define.h"
#include <fstream>
#include <ShlObj.h>


#define LOGIN_FILE              _T("login.xml")
#define LOGIN_CLASS_NAME        _T("login_class")
#define CLOSE_BTN_NAME          _T("closebtn")
#define CANCLE_BTN_NAME         _T("cancelbtn")
#define LOGIN_BTN_NAME          _T("loginbtn")
#define USERNAME_EDIT_NAME      _T("username")
#define PASSWD_EDIT_NAME        _T("password")
#define SAVEINFO_CHECK_NAME     _T("saveaccount")
#define USERNAME_COMBO_NAME     _T("username_combo")
#define TOOLTIPS_NAME           _T("tooltips")
#define ACCOUNT_FILE            _T("\\accounts")


#define USERNAME_NODE           _T("username")
#define PASSWORD_NODE           _T("password")

#define USERS_NODE              _T("users")
#define CURRENT_USER_NODE       _T("current_user_index")




#define LOGINING_TOOLTIPS         _T("正在登录...")
#define LOGIN_FAILED_TOOLTIPS     _T("用户名/密码错误")

BEGIN_APP_NAMESPACE


#define WM_TAB_KEY_PRESS   (WM_USER + 1)
#define WM_ENTER_KEY_PRESS (WM_USER + 2)

HHOOK LoginUI::hook;
HWND  LoginUI::hwnd;

LoginUI::LoginUI() 
	: m_pLogSystem(NULL)
	, m_pNetService(NULL)
	, m_pLocalConfig(NULL)
	, m_pPasswd(NULL)
	, m_pUserName(NULL)
	, m_pComboUserName(NULL)
{
	CreateDuiWindow(NULL, LOGIN_CLASS_NAME);
}


LoginUI::~LoginUI()
{
}
void LoginUI::Init(INetService* pNetService, ILocalConfigService* pConfig, ILogSystem* pLogSystem)
{
	m_pNetService = pNetService;
	m_pLocalConfig = pConfig;
	m_pLogSystem = pLogSystem;

	LoadUserToUI();
}

DuiLib::CDuiString LoginUI::GetSkinFolder()
{
	return SKIN_FOLDER;
}

DuiLib::CDuiString LoginUI::GetSkinFile()
{
	return LOGIN_FILE;
}

LPCTSTR LoginUI::GetWindowClassName(void) const
{
	return LOGIN_CLASS_NAME;
}

void LoginUI::OnFinalMessage(HWND hWnd)
{
	if (NULL != hook){
		::UnhookWindowsHookEx(hook);
		hook = NULL;
	}
	delete this;
}

void LoginUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		tstring strName = msg.pSender->GetName().GetData();
		if (strName == CLOSE_BTN_NAME || strName == CANCLE_BTN_NAME){
			Close(IDCANCEL);
		}
		else if (strName == LOGIN_BTN_NAME){
			tstring strUserName = m_pUserName->GetText().GetData();
			tstring strPasswd = m_pPasswd->GetText().GetData();
			bool bIsRemeberUserAccount = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(SAVEINFO_CHECK_NAME))->IsSelected();

			if (strUserName.empty() || strPasswd.empty()){
				MessageBox(NULL, _T("用户名或密码不能为空！"), _T("INFO"), 0);
				return;
			}
			EnsureControlStatus(false, true, LOGINING_TOOLTIPS);
			m_loginThread = boost::thread(boost::bind(&LoginUI::Login, this, strUserName, strPasswd, bIsRemeberUserAccount));
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT&& msg.pSender == m_pComboUserName){
		int nIndex = m_pComboUserName->GetCurSel();
		if (nIndex >= 0 && nIndex < m_users.size()){
			const tstring& strName = m_users[nIndex].strName;
			const tstring& strPasswd = m_users[nIndex].strPasswd;

			if (m_pUserName->GetText() != strName.c_str()){
				m_pUserName->SetText(strName.c_str());
			}
			if (m_pPasswd->GetText() != strName.c_str()){
				m_pPasswd->SetText(strPasswd.c_str());
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED && msg.pSender == m_pUserName){
		tstring strName = m_pUserName->GetText().GetData();
		if (m_mapUserIndex.find(strName) != m_mapUserIndex.end()){
			int nIndex = m_mapUserIndex[strName];
			m_pComboUserName->SelectItem(nIndex);
		}
		else {
			m_pComboUserName->SelectItem(-1);
		}
	}
}

void LoginUI::Log(LOG_TYPE logType, const tstring& strText)
{
	if (logType > LOG_TYPE::DEBUG_LOG){

	}
}



LRESULT LoginUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_loginThread.get_id() != boost::thread::id()){
		return 0;
	}
	if (uMsg == WM_TAB_KEY_PRESS){
		if (m_pUserName->GetNativeWindow() == NULL){
			m_pUserName->SetFocus();
			m_pUserName->SetSelAll();
		}
		else if (m_pPasswd->GetNativeWindow() == NULL){
			m_pPasswd->SetFocus();
			m_pPasswd->SetSelAll();
		}
		bHandled = TRUE;
	}
	else if (uMsg == WM_ENTER_KEY_PRESS){
		m_PaintManager.SendNotify(m_PaintManager.FindControl(LOGIN_BTN_NAME), DUI_MSGTYPE_CLICK);
		bHandled = TRUE;
	}
	return 0L;
}

void LoginUI::EnsureControlStatus(bool bRelativeCtrlStatus, bool bTooltipsVisibled, const tstring& strTooltips)
{
	static const TCHAR* relative_ctrls[] = {
		CLOSE_BTN_NAME,
		USERNAME_COMBO_NAME,
		USERNAME_EDIT_NAME,
		PASSWD_EDIT_NAME,
		SAVEINFO_CHECK_NAME,
		LOGIN_BTN_NAME,
		CANCLE_BTN_NAME
	};
	for (size_t i = 0; i < _countof(relative_ctrls); ++i){
		CControlUI* pControl = m_PaintManager.FindControl(relative_ctrls[i]);
		if (pControl){
			pControl->SetEnabled(bRelativeCtrlStatus);
		}
	}
	CControlUI* pTooltips = m_PaintManager.FindControl(TOOLTIPS_NAME);
	if (pTooltips){
		pTooltips->SetVisible(bTooltipsVisibled);
		pTooltips->SetText(strTooltips.c_str());
	}
}

void LoginUI::Login(const tstring& strUserName, const tstring& strPasswd, bool bRemeber)
{
	bool bRet = false;
	if (m_pNetService != NULL){
		if (m_pNetService->Login(strUserName, strPasswd)){
			if (bRemeber){
				SerializeUsers();
			}
			bRet = true;
			Close(IDOK);
		}
	}
	if (!bRet){
		EnsureControlStatus(true, true, LOGIN_FAILED_TOOLTIPS);
	}
}

LRESULT LoginUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_ESCAPE)
	{
		Close(IDCANCEL);
		return TRUE;
	}
	return FALSE;
}

void LoginUI::SerializeUsers()
{
	if (m_pLocalConfig == nullptr)return;

	const tstring strName = m_pUserName->GetText().GetData();
	const tstring strPasswd = m_pPasswd->GetText().GetData();

	int nIndex = -1;

	if (m_mapUserIndex.find(strName) != m_mapUserIndex.end()){
		nIndex = m_mapUserIndex[strName];
		m_users[nIndex].strPasswd = strPasswd;
	}
	else {
		USER_INFO userInfo;
		userInfo.strName = strName;
		userInfo.strPasswd = strPasswd;
		m_users.push_back(userInfo);
		nIndex = m_users.size() - 1;
	}

	Json::Value jsonLogin;

	jsonLogin[CURRENT_USER_NODE] = nIndex;
	
	Json::Value jsonUsers;
	for (size_t i = 0; i < m_users.size(); ++i){
		Json::Value jsonCurrentUser;
		jsonCurrentUser[USERNAME_NODE] = m_users[i].strName;
		jsonCurrentUser[PASSWORD_NODE] = m_users[i].strPasswd;
		jsonUsers.append(jsonCurrentUser);
	}
	jsonLogin[USERS_NODE] = jsonUsers;

	m_pLocalConfig->Serialize(jsonLogin);
}

void LoginUI::LoadUserToUI()
{
	if (m_pComboUserName == nullptr
		|| m_pLocalConfig == nullptr){
		return;
	}
	m_pComboUserName->RemoveAll();
	m_users.clear();
	m_mapUserIndex.clear();

	Json::Value  jsonLogin = m_pLocalConfig->Deserialize();
	if (jsonLogin.isNull())return;

	Json::Value& jsonUsers = jsonLogin[USERS_NODE];
	Json::Value& currentIndex = jsonLogin[CURRENT_USER_NODE];

	for (size_t i = 0; i < jsonUsers.size(); ++i){
		const Json::Value& jsonCurrentUser = jsonUsers[i];
		USER_INFO userInfo;
		try {
			userInfo.strName = jsonCurrentUser[USERNAME_NODE].asString();
			userInfo.strPasswd = jsonCurrentUser[PASSWORD_NODE].asString();
		}
		catch (...){
			continue;
		}
		m_users.push_back(userInfo);
	}

	for (size_t i = 0; i < m_users.size(); ++i){
		USER_INFO& userInfo = m_users[i];

		CListLabelElementUI* pListLabel = new CListLabelElementUI();
		pListLabel->SetText(userInfo.strName.c_str());
		m_pComboUserName->Add(pListLabel);
		m_mapUserIndex[m_users[i].strName] = pListLabel->GetIndex();
	}
	try {
		int nIndex = currentIndex.asInt();
		if (nIndex < 0)nIndex = 0;
		if (nIndex >= 0 && nIndex < m_users.size()){
			m_pComboUserName->SelectItem(nIndex);
			m_pUserName->SetText(m_users[nIndex].strName.c_str());
			m_pPasswd->SetText(m_users[nIndex].strPasswd.c_str());
		}
	}
	catch (...){

	}
}


void LoginUI::InitWindow()
{

	m_pComboUserName = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(USERNAME_COMBO_NAME));
	m_pUserName = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(USERNAME_EDIT_NAME));
	m_pPasswd = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(PASSWD_EDIT_NAME));

	hwnd = GetHWND();
	hook = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)&LoginUI::KeyboardProc, (HINSTANCE)NULL, ::GetCurrentThreadId());
}

LRESULT CALLBACK LoginUI::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static LONG_PTR lVkCodePrev_Tab = 0;
	static LONG_PTR lVkCode_Tab = 0;

	static LONG_PTR lVkCode_Enter = 0;
	static LONG_PTR lVkCode_Cancel = 0;

	LONG_PTR        lTemp = 0;

	/** when tab key press down and up
	KeyboardProc : nCode = 0x0, wParam = 0x9, lParam = 0xF0001
	KeyboardProc : nCode = 0x0, wParam = 0x9, lParam = 0xC00F0001
	*/

	if (0 == nCode)
	{
		switch (wParam)
		{
		case VK_TAB:
		{
			 lVkCode_Tab = (LONG_PTR)lParam;
			 lTemp = lVkCode_Tab & 0xFFF00000;
			 if (0 == lTemp)
			 {
			   // tab key press down  
			   lVkCodePrev_Tab = lVkCode_Tab;
			 }
			 else
			 {
			   // tab key press up  
			   if (lVkCode_Tab != lVkCodePrev_Tab)
			   {
				   lVkCodePrev_Tab = lVkCode_Tab;
				   if (NULL != LoginUI::hwnd)
				   {
					   ::PostMessageW(LoginUI::hwnd, WM_TAB_KEY_PRESS, 0, 0);
				   }
			   }
			 }
		}
			break;
		case VK_RETURN:
		{
			lVkCode_Enter = (LONG_PTR)lParam;
			lTemp = lVkCode_Enter & 0xFFF00000;
			if (lTemp > 0)
			{
			  // key press up  
				if (NULL != LoginUI::hwnd)
			  {
					::PostMessageW(LoginUI::hwnd, WM_ENTER_KEY_PRESS, 0, 0);
			  }
			}
		}
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(LoginUI::hook, nCode, wParam, lParam);
}
END_APP_NAMESPACE
