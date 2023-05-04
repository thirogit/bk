#ifndef __CONTEXTINITIALISER_H__
#define __CONTEXTINITIALISER_H__

#include "XContext.h"


class XContextInitialiser
{
public:
	XContextInitialiser(XContext* pContext);
	void Init();
	void SetServer(const std::wstring& sAddress,uint16_t port);
	void SetFarm(const FarmNo& farmNo);
	void SetLoginCredentials(const std::wstring& sLogin,const std::wstring& sPassword);
	void LoadSettings(SimpleProgress* progress);

private:
	XContext* m_pContext;

	std::wstring m_sAddress;
	uint16_t m_Port;
	std::wstring m_sLogin;
	std::wstring m_sPassword;
	FarmNo m_farmNo;
};

#endif