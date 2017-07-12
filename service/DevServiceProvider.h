#pragma once

#include "interface/IDevService.h"
#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"
#include "interface/ILogSystem.h"
#include "interface/device/IEfirm.h"
#include "interface/device/IReadWriteIO.h"
#include "x3py/interface/core/objptr.h"

using namespace ezlife_dev;

BEGIN_APP_NAMESPACE

class DevProvider 
	: public IDevService
	, public IServiceProvider
	, public IServiceConsumer
{
public:
	DevProvider();
	virtual ~DevProvider();
	virtual bool         GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;

	virtual tstring      GetProviderName() const override;

	virtual void         ConsumeService(IServiceProvider* pServiceProvider) override;

public:
	virtual bool         ConnectDevice() override;
	virtual bool         DisconnectDevice() override;
	virtual bool         WriteBarcode(const tstring& strBarcode) override;
	virtual tstring      ReadBarcode() override;
	virtual tstring      GetEplateType() const;

private:
	ILogSystem*      m_pLogSystem;

	IReadWriteIO* GetUSBIO();
	IEfirm*       GetEfirm96();
	IEfirm*       GetEfirm16();
private:
	tstring          m_strUsbDescription;
	tstring          m_strUsbVID;
	tstring          m_strUsbPID;
	tstring          m_strComVID;
	tstring          m_strComPID;
	tstring          m_strEfirmType;
private:
	x3::Object<IEfirm>   m_efirm96;
	x3::Object<IEfirm>   m_efirm16;
	x3::Object<IReadWriteIO> m_usbIO;

	IEfirm*              m_pCurrentEfirm;
};
END_APP_NAMESPACE