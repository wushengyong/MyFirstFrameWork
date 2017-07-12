#pragma once
#include "define.h"
#include "interface/ILogger.h"
#include "interface/ILogSystem.h"
#include "interface/IDevService.h"
#include "interface/INetService.h"

#define WM_MAINVIEW_SWITCH_USER    (UINT)-1
BEGIN_APP_NAMESPACE

class MainUI
	: public WindowImplBase
	, public ILogger
{
public:
	MainUI();
	~MainUI();

	void   Init(ILogSystem* pLogSystem, IDevService* pDevService, INetService* pNetService);
	virtual CDuiString GetSkinFolder() override;

	virtual CDuiString GetSkinFile() override;

	virtual LPCTSTR GetWindowClassName(void) const override;

	virtual void Notify(TNotifyUI& msg) override;

	virtual void Log(LOG_TYPE logType, const tstring& strLog) override;

	virtual void InitWindow() override;

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual void OnFinalMessage(HWND hWnd) override;

	void EnsureControlStatus(bool bEnabled);

	void    WriteBarcode(const tstring& strBarcode, const tstring& strEplateType);
	void    ReadBarcode();
	void    OnDeviceOp();
protected:
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	void  LoadBarcodeFromUI();
	void  IncrementCurrentSn();

private:
	CComboUI*       m_pEplateType;
	CComboUI*       m_pObjecType;
	CComboUI*       m_pProjectType;
	CComboUI*       m_pTargetType;
	CEditUI*        m_pDate;
	CComboUI*       m_pBatch;
	CEditUI*        m_pCurrentSn;
	CEditUI*        m_pBarcodeFromUI;
	CEditUI*        m_pBarcodeFromDevice;
	CRichEditUI*    m_pLogPad;
	COptionUI*      m_pDeviceBtn;
private:
	ILogSystem*     m_pLogSystem;
	IDevService*    m_pDevService;
	INetService*    m_pNetService;

private:
	int             m_nCurrentSn;
};

END_APP_NAMESPACE