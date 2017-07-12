#include "stdafx.h"
#include "MainUI.h"
#include "project_define.h"
#include "base/ptimetostring.h"
#include "common/DateTimeTools.h"
#include "boost/thread.hpp"

#define MAIN_VIEW_XML           _T("mainview.xml")
#define MAIN_VIEW_CLASS_NAME    _T("mainview_class")
#define LOGOUT_BTN_NAME         _T("logout")
#define CLOSE_BTN_NAME          _T("closebtn")
#define DEVICE_BTN_NAME         _T("connect_device")

#define EPLATE_TYPE_NAME        _T("eplate_type")
#define PROJECT_TYPE_NAME        _T("project_type")
#define OBJECT_TYPE_NAME        _T("object_type")
#define TARGET_TYPE_NAME        _T("target_type")
#define DATA_NAME               _T("date")
#define BATCH_NAME              _T("batch")
#define CURRENT_SN_NAME         _T("current_sn")
#define BARCODE_FROM_UI         _T("barcode_from_ui")
#define BARCODE_FROM_DEVICE     _T("barcode_from_device")
#define READ_BARCODE            _T("read_barcode")
#define WRITE_BARCODE           _T("write_barcode")
#define LOGS_PAD                _T("logs")

#define DEVICE_BTN_TOOLTIP_SELECTED      _T("断开设备")
#define DEVICE_BTN_TOOLTIP_UNSELECTED    _T("连接设备")
#define WM_LOGS                          (WM_USER + 1)


enum {
	EPLATE_TYPE_96WELL,
	EPLATE_TYPE_16WELL,
	EPLATE_TYPE_16x3WELL,
};

enum {
	PROJECT_TYPE_MEDICAL,
	PROJECT_TYPE_QUARANTINE,
	PROJECT_TYPE_PRODUCE_SECURITY,
	PROJECT_TYPE_99MALL,
	PROJECT_TYPE_TEST_BOARD,
};

enum {
	OBJEC_TYPE_NUCLEIC_ACID,
	OBJECT_TYPE_PROTEIN,
	OBJECT_TYPE_TEST_BOARD,
};

enum {
	TARGET_TYPE_EGFR,
	TARGET_TYPE_ZIKA,
	TARGET_TYPE_ALDH2,
};


#define MIN_BATCH  0
#define MAX_BATCH  99

#define BEGIN_SN   1
#define MAX_SN     999

BEGIN_APP_NAMESPACE

MainUI::MainUI()
: m_pEplateType(NULL)
, m_pObjecType(NULL)
, m_pTargetType(NULL)
, m_pDate(NULL)
, m_pBatch(NULL)
, m_pCurrentSn(NULL)
, m_pBarcodeFromUI(NULL)
, m_pBarcodeFromDevice(NULL)
, m_pLogPad(NULL)
, m_pDevService(NULL)
, m_pLogSystem(NULL)
, m_nCurrentSn(BEGIN_SN)
, m_pDeviceBtn(NULL)
, m_pNetService(NULL)
{
	CreateDuiWindow(NULL, MAIN_VIEW_CLASS_NAME);
}


MainUI::~MainUI()
{
	if (m_pLogSystem){
		m_pLogSystem->RemoveLogger(this);
	}
	if (m_pDevService){
		m_pDevService->DisconnectDevice();
	}
}

void MainUI::Init(ILogSystem* pLogSystem, IDevService* pDevService, INetService* pNetService)
{
	m_pLogSystem = pLogSystem;
	m_pDevService = pDevService;
	m_pNetService = pNetService;

	m_pLogSystem->AddLogger(this);

	// 设置barcode
	LoadBarcodeFromUI();
}

DuiLib::CDuiString MainUI::GetSkinFolder()
{
	return SKIN_FOLDER;
}

DuiLib::CDuiString MainUI::GetSkinFile()
{
	return MAIN_VIEW_XML;
}

LPCTSTR MainUI::GetWindowClassName(void) const
{
	return MAIN_VIEW_CLASS_NAME;
}

void MainUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		tstring strName = msg.pSender->GetName();

		if (LOGOUT_BTN_NAME == strName){
			Close(WM_MAINVIEW_SWITCH_USER);
			return;
		}
		else if (CLOSE_BTN_NAME == strName){
			Close(IDOK);
			return;
		}
		else if (WRITE_BARCODE == strName){
			tstring strBarcode = m_pBarcodeFromUI->GetText().GetData();
			EnsureControlStatus(false);
			boost::thread threadThread = boost::thread(boost::bind(&MainUI::WriteBarcode, this, strBarcode, m_pDevService->GetEplateType()));

		}
		else if (READ_BARCODE == strName){
			EnsureControlStatus(false);
			boost::thread threadThread = boost::thread(boost::bind(&MainUI::ReadBarcode, this));
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT ||
		msg.sType == DUI_MSGTYPE_TEXTCHANGED){
		if (msg.pSender == m_pEplateType || msg.pSender == m_pObjecType ||
			msg.pSender == m_pProjectType || msg.pSender == m_pTargetType ||
			msg.pSender == m_pDate || msg.pSender == m_pBatch ||
			msg.pSender == m_pCurrentSn){
			LoadBarcodeFromUI();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED){
		if (msg.pSender == m_pDeviceBtn){
			EnsureControlStatus(false);
			boost::thread threadThread = boost::thread(boost::bind(&MainUI::OnDeviceOp, this));
		}
	}
}

void MainUI::EnsureControlStatus(bool bEnabled)
{
	const TCHAR* controls[] = {
		DEVICE_BTN_NAME,
		LOGOUT_BTN_NAME,
		CLOSE_BTN_NAME,
		READ_BARCODE,
		WRITE_BARCODE
	};
	for (size_t i = 0; i < _countof(controls); ++i){
		CControlUI* pControlUI = m_PaintManager.FindControl(controls[i]);
		if (pControlUI){
			pControlUI->SetEnabled(bEnabled);
		}
	}
}

void MainUI::WriteBarcode(const tstring& strBarcode, const tstring& strEplateType)
{
	if (m_pDevService->WriteBarcode(strBarcode) &&
		m_pNetService->CreateEplate(strBarcode, strEplateType)){
		IncrementCurrentSn();
	}
	EnsureControlStatus(true);
}

void MainUI::OnDeviceOp()
{
	if (m_pDeviceBtn->IsSelected()){
		// 连接设备
		if (m_pDevService->ConnectDevice() == false){
			m_pDeviceBtn->Selected(false, false);
		}
		else {
			m_pDeviceBtn->SetToolTip(DEVICE_BTN_TOOLTIP_SELECTED);
		}
	}
	else {
		if (m_pDevService->DisconnectDevice() == false){
			m_pDeviceBtn->Selected(true, false);
		}
		else {
			m_pDeviceBtn->SetToolTip(DEVICE_BTN_TOOLTIP_UNSELECTED);
		}
	}
	EnsureControlStatus(true);
}

void app::MainUI::ReadBarcode()
{
	tstring strBarcode = m_pDevService->ReadBarcode();
	m_pBarcodeFromDevice->SetText(strBarcode.c_str());
	EnsureControlStatus(true);
}

void app::MainUI::InitWindow()
{
	m_pEplateType = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(EPLATE_TYPE_NAME));
	m_pObjecType = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(OBJECT_TYPE_NAME));
	m_pProjectType = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(PROJECT_TYPE_NAME));
	m_pTargetType = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(TARGET_TYPE_NAME));
	m_pDate = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(DATA_NAME));
	m_pBatch = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(BATCH_NAME));
	m_pCurrentSn = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(CURRENT_SN_NAME));
	m_pBarcodeFromUI = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(BARCODE_FROM_UI));
	m_pBarcodeFromDevice = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(BARCODE_FROM_DEVICE));
	m_pLogPad = dynamic_cast<CRichEditUI*>(m_PaintManager.FindControl(LOGS_PAD));
	m_pDeviceBtn = dynamic_cast<COptionUI*>(m_PaintManager.FindControl(DEVICE_BTN_NAME));

	// 设置项目批次

	for (size_t i = MIN_BATCH; i <= MAX_BATCH; ++i){
		CDuiString strText;
		strText.Format(_T("%02d"), i);
		CListLabelElementUI* pListLabel = new CListLabelElementUI();
		pListLabel->SetText(strText);
		m_pBatch->Add(pListLabel);
	}
	m_pBatch->SelectItem(0);

	if (m_pCurrentSn){
		CDuiString strText;
		strText.Format(_T("%03d"), m_nCurrentSn);
		m_pCurrentSn->SetText(strText);
	}

	// 设置日期

	SYSTEMTIME sys = { 0 };
	GetLocalTime(&sys);
	CDuiString strDate;
	strDate.Format(_T("%02d%02d"), sys.wYear % 100, sys.wMonth);
	m_pDate->SetText(strDate);
}

void app::MainUI::Log(LOG_TYPE logType, const tstring& strLog)
{
	if (logType > LOG_TYPE::DEBUG_LOG && m_pLogPad){
		CDuiString strAppendLogs = (GetCurrentDateTimeStr() + _T(" ") + strLog).c_str();
		CDuiString* pLogs = new CDuiString(strAppendLogs);
		PostMessage(WM_LOGS, (WPARAM)pLogs);
	}
}

LRESULT MainUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_ESCAPE)
	{
		Close(IDCANCEL);
		return TRUE;
	}
	return FALSE;
}



void MainUI::LoadBarcodeFromUI()
{
	int nEplateType = m_pEplateType->GetCurSel();
	int nObjectType = m_pObjecType->GetCurSel();
	int nProjectType = m_pProjectType->GetCurSel();
	int nTargetType = m_pTargetType->GetCurSel();
	const tstring strDate = m_pDate->GetText();
	const tstring strBatch = m_pBatch->GetText();
	const tstring strCurrent = m_pCurrentSn->GetText();

	tstring strBarcode;

	switch (nEplateType){
	case EPLATE_TYPE_96WELL:strBarcode += _T("1"); break;
	case EPLATE_TYPE_16WELL:strBarcode += _T("2");break;
	case EPLATE_TYPE_16x3WELL:strBarcode += _T("3"); break;
	default:return;
	}

	switch (nObjectType)
	{
	case OBJEC_TYPE_NUCLEIC_ACID:strBarcode += _T("A"); break;
	case OBJECT_TYPE_PROTEIN:strBarcode += _T("B"); break;
	case OBJECT_TYPE_TEST_BOARD:strBarcode += _T("F"); break;
	default:return;
	}

	switch (nProjectType)
	{
	case PROJECT_TYPE_MEDICAL:strBarcode += _T("00"); break;
	case PROJECT_TYPE_QUARANTINE:strBarcode += _T("01"); break;
	case PROJECT_TYPE_PRODUCE_SECURITY:strBarcode += _T("02"); break;
	case PROJECT_TYPE_99MALL:strBarcode += _T("03"); break;
	case PROJECT_TYPE_TEST_BOARD:strBarcode += _T("FF"); break;
	default:return;
	}

	switch (nTargetType){
	case TARGET_TYPE_EGFR:strBarcode += _T("001"); break;
	case TARGET_TYPE_ZIKA:strBarcode += _T("002"); break;
	case TARGET_TYPE_ALDH2:strBarcode += _T("003"); break;
	default:
		return;
	}
	strBarcode += strDate;
	strBarcode += strBatch;
	strBarcode += strCurrent;

	m_pBarcodeFromUI->SetText(strBarcode.c_str());
}


void MainUI::IncrementCurrentSn()
{
	if (m_nCurrentSn >= MAX_SN){
		CDuiString strText;
		strText.Format(_T("Current SN is bigger than %03d. Reset to %03d."), MAX_SN, BEGIN_SN);
		m_pLogSystem->Log(LOG_TYPE::WARNING_LOG, strText.GetData());
		m_nCurrentSn = BEGIN_SN;
	}
	else {
		++m_nCurrentSn;
	}
	if (m_pCurrentSn){
		CDuiString strText;
		strText.Format(_T("%03d"), m_nCurrentSn);
		m_pCurrentSn->SetText(strText);
	}
}
LRESULT MainUI::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_LOGS){
		CDuiString* pAppendLogs = (CDuiString*)wParam;
		CDuiString& strAppendLogs = *pAppendLogs;
		if (m_pLogPad->GetText().IsEmpty()){
			m_pLogPad->SetText(strAppendLogs);
		}
		else {
			m_pLogPad->SetText(m_pLogPad->GetText() + _T("\n") + strAppendLogs);
		}
		m_pLogPad->EndDown();

		delete pAppendLogs;

		bHandled = TRUE;
	}

	return 0;
}

void MainUI::OnFinalMessage(HWND hWnd)
{
	delete this;
}
END_APP_NAMESPACE


