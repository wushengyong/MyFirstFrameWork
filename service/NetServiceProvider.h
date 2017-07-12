#pragma once

#include "interface/INetService.h"
#include "interface/IServiceProvider.h"
#include "interface/IServiceConsumer.h"
#include "interface/network/INetComm.h"
#include "interface/ILogSystem.h"

using namespace ezlife_net;

BEGIN_APP_NAMESPACE

class NetProvider 
	: public INetService
	, public IServiceProvider
	, public IServiceConsumer
{
public:
	NetProvider();
	virtual ~NetProvider();
	virtual bool      GetService(const tstring& strConsumer, const tstring& strServiceProvider, const tstring& strServiceName, void** pService) override;
	virtual tstring   GetProviderName() const override;

	virtual void      ConsumeService(IServiceProvider* pServiceProvider) override;

public:
	virtual bool               Login(const tstring& strUserName, const tstring& strPasswd) override;
	virtual bool               Logout() override;
	virtual bool               CreateEplate(const tstring& strBarcode, const tstring& strEplateType) override;

	ezlife_net::INetComm*      GetNetCmm();


private:
	void Debug(const tstring& strText);
	void Warning(const tstring& strText);
	void Error(const tstring& strText);
	void Info(const tstring& strText);

private:
	tstring                           m_strToken;
	tstring                           m_strServerUrl;
	x3::Object<ezlife_net::INetComm>  m_netCmm;
	ILogSystem*                       m_pLogSystem;

};
END_APP_NAMESPACE