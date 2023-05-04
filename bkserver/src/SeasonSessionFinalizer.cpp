#include "stdafx.h"
#include "SeasonSessionFinalizer.h"
#include "SeasonSession.h"
#include <arrays/DeepDelete.h>
#include "seasonimpl/ClientSeasonSessionInterfaceImpl.h"

SeasonSessionFinalizer::SeasonSessionFinalizer(SeasonSession* session) : m_session(session)
{
}

void SeasonSessionFinalizer::Finalize()
{
	//std::set<FetchHandleImpl*> m_FetchHandles;

	//std::set<ClientSeasonSession*> m_SeasonClients;
	
	m_session->m_service.stop();
	m_session->m_worker.join();
	
	delete m_session->m_pClientInterface;
	delete m_session->m_logger;
	delete m_session->m_pChangePublisher;
	
	DeepDelete(m_session->m_Hents);	

	DeepDelete(m_session->m_Cows);
		
	DeepDelete(m_session->m_Registers);

	DeepDelete(m_session->m_InDocs);
	DeepDelete(m_session->m_OutDocs);
	DeepDelete(m_session->m_BuyDocs);
	DeepDelete(m_session->m_SellDocs);
	DeepDelete(m_session->m_MoveDocs);
	DeepDelete(m_session->m_BuyInvoices);
	DeepDelete(m_session->m_SellInvoices);
		
	
	
	
	
	

	
}
