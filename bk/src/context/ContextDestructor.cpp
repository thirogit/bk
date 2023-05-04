#include "stdafx.h"
#include "ContextDestructor.h"
#include "ContextException.h"
#include <arrays/DeepDelete.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

XContextDestructor::XContextDestructor(XContext* pContext) : m_pContext(pContext)
{
}

void XContextDestructor::Destroy()
{
	if(m_pContext->m_pClient != NULL)
	{
		try
		{
			m_pContext->SaveSettings();
		}
		catch (...)	
		{
			
		}
	
		RCF::ClientStubPtr clientStub = m_pContext->m_pClient->getClientStubPtr();
		if(clientStub->isConnected())
		{
			clientStub->disconnect();
		}

		delete m_pContext->m_pClient;
		m_pContext->m_pClient = NULL;
	}

	delete m_pContext->m_pFarm;
	delete m_pContext->m_User;
	DeepDelete(m_pContext->m_Herds);
	DeepDelete(m_pContext->m_Seasons);
	DeepDelete(m_pContext->m_Agents);
	DeepDelete(m_pContext->m_Accounts);
	DeepDelete(m_pContext->m_printProfiles);	 
	DeepDelete(m_pContext->m_passRecgntnProfiles);
	DeepDelete(m_pContext->m_deductionDefinitions);
	DeepDelete(m_pContext->m_Classes);
	DeepDelete(m_pContext->m_Stocks);
	DeepDelete(m_pContext->m_settings);
	delete m_pContext->m_pSettingsFacade;
}

