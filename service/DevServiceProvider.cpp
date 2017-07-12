#include "stdafx.h"
#include "DevServiceProvider.h"
#include "interface/IConfigService.h"
#include "json/json.h"
#include "interface/device/ftd2xx.h"


#define DEVICE_NODE           _T("device")
#define USB_NODE              _T("usb")
#define COM_NODE              _T("com")
#define DESCRIPTION_NOE       _T("description")
#define PID_NODE              _T("pid")
#define VID_NODE              _T("vid")


#define EFIRM96_CLSID   "9F73C37B-0DD3-4784-BA67-D32D8A9B5D18"
#define EFIRM16_CLSID   "70B3A7E9-69CF-4D4A-B6DB-C950F4B583EF"
#define USBIO_CLSID     "DCAD22BC-082B-44D8-9C1B-3775CA023828"

#define EFIRM_TYPE_96WELLS       _T("eplate_96_wells")
#define EFIRM_TYPE_16WELLS       _T("eplate_16_wells")

BEGIN_APP_NAMESPACE

DevProvider::DevProvider()
: m_pLogSystem(NULL)
, m_pCurrentEfirm(NULL)
{

}



DevProvider::~DevProvider()
{
	DisconnectDevice();
}

bool DevProvider::GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService)
{
	if (DEV_PROVIDER_NAME == strServiceProvider && DEV_SERVICE_NAME == strServiceName) {
		*pService = (void*)this;
		return true;
	}
	return false;
}

tstring DevProvider::GetProviderName() const
{
	return DEV_PROVIDER_NAME;
}



void DevProvider::ConsumeService(IServiceProvider* pServiceProvider)
{
	pServiceProvider->GetService(DEV_PROVIDER_NAME, LOG_PROVIDE_NAME, LOG_SERVICE_NAME, (void**)&m_pLogSystem);
	IGlobalConfigService* pGlobalConfigService = nullptr;
	pServiceProvider->GetService(DEV_PROVIDER_NAME, CONFIG_RPOVIDER_NAME, GLOBAL_CONFIG_SERVICE, (void**)&pGlobalConfigService);
	if (pGlobalConfigService){
		try {
			Json::Value jsonDevice = pGlobalConfigService->GetConfigNode(DEVICE_NODE);
			Json::Value& usb = jsonDevice[USB_NODE];
			Json::Value& com = jsonDevice[COM_NODE];
			m_strUsbDescription = usb[DESCRIPTION_NOE].asString();
			m_strUsbPID = usb[PID_NODE].asString();
			m_strUsbVID = usb[VID_NODE].asString();
			m_strComPID = com[PID_NODE].asString();
			m_strComVID = com[VID_NODE].asString();
		}
		catch (...){}

	}
}


bool DevProvider::DisconnectDevice()
{
	bool bRet = true;
	if (m_pCurrentEfirm){
		bRet = (DEV_OK == m_pCurrentEfirm->Disconnected());
		if (bRet){
			m_pCurrentEfirm = NULL;
			m_strEfirmType = _T("");
			m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, _T("成功断开设备！"));
		}
		else {
			m_pLogSystem->Log(LOG_TYPE::ERROR_LOG, _T("无法断开设备！"));
		}
	}
	return bRet;
}

tstring DevProvider::ReadBarcode()
{
	if (m_pCurrentEfirm == nullptr){
		m_pLogSystem->Log(LOG_TYPE::WARNING_LOG, _T("未连接设备，请连接!"));
		return _T("");
	}
	tstring strBarcode;
	if (DEV_OK != m_pCurrentEfirm->GetEplateBarcode(strBarcode)){
		m_pLogSystem->Log(LOG_TYPE::ERROR_LOG, _T("已经连接设备，但是无法获取barcode"));
		return _T("");
	}
	m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, _T("成功获取Barcode : ") + strBarcode);
	return strBarcode;
}


tstring DevProvider::GetEplateType() const
{
	return m_strEfirmType;
}

IReadWriteIO* DevProvider::GetUSBIO()
{
	if (m_usbIO.p() == nullptr){
		m_usbIO = x3::Object<IReadWriteIO>(USBIO_CLSID);
	}
	return m_usbIO.p();
}
IEfirm* DevProvider::GetEfirm16()
{
	if (m_efirm16.p() == nullptr){
		m_efirm16 = x3::Object<IEfirm>(EFIRM16_CLSID);
	}
	return m_efirm16.p();
}
IEfirm* DevProvider::GetEfirm96()
{
	if (m_efirm96.p() == nullptr){
		m_efirm96 = x3::Object<IEfirm>(EFIRM96_CLSID);
	}
	return m_efirm96.p();
}

bool DevProvider::WriteBarcode(const tstring& strBarcode)
{
	if (m_pCurrentEfirm == nullptr){
		m_pLogSystem->Log(LOG_TYPE::WARNING_LOG, _T("未连接设备，请连接!"));
		return false;
	}
	if (DEV_OK != m_pCurrentEfirm->SetEplateBarcode(strBarcode)){
		m_pLogSystem->Log(LOG_TYPE::ERROR_LOG, _T("已经连接设备，但是未能成功设置Barcode : ") + strBarcode);
		return false;
	}
	m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, _T("成功写入Barcode : ") + strBarcode);
	return true;
}

bool DevProvider::ConnectDevice()
{
	if (m_pCurrentEfirm){
		DisconnectDevice();
	}
	IReadWriteIO* pUsbIO = GetUSBIO();
	IEfirm* pEfirm96 = GetEfirm96();

	if (DEV_OK == pUsbIO->Open(m_strUsbDescription, FT_OPEN_BY_DESCRIPTION)){
		m_pLogSystem->Log(LOG_TYPE::DEBUG_LOG, _T("Succeed Open Device (USB IO)."));

		pEfirm96->Connected(pUsbIO, IO_USB);
		EfirmType type = EFIRM_UNKOWN;

		if (DEV_OK == pEfirm96->GetEfirmType(type)){
			if (EFIRM_96_CHANNELS == type){
				m_pCurrentEfirm = pEfirm96;
				m_strEfirmType = EFIRM_TYPE_96WELLS;
				m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, _T("Succeed Connect Efirm 96."));
				return true;
			}
			else if (EFIRM_16_CHANNELS == type){
				pEfirm96->Disconnected();
				m_pCurrentEfirm = GetEfirm16();
				m_strEfirmType = EFIRM_TYPE_16WELLS;
				m_pCurrentEfirm->Connected(pUsbIO, IO_USB);
				m_pLogSystem->Log(LOG_TYPE::NORMAL_LOG, _T("Succeed Connect Efirm 16."));
				return true;
			}
			else {
				m_pLogSystem->Log(LOG_TYPE::DEBUG_LOG, _T("Unknown Type."));
			}
		}
		
	}
	else {
		m_pLogSystem->Log(LOG_TYPE::DEBUG_LOG, _T("Cannot Open Device (USB IO)."));
	}
	m_pLogSystem->Log(LOG_TYPE::ERROR_LOG, _T("Cannot Connect Efirm."));
	return false;

}


END_APP_NAMESPACE