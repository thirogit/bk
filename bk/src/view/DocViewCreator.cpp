#include "stdafx.h"
#include "DocViewCreator.h"
#include "../text/DocNoText.h"
#include <utils\SafeGet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DocViewCreator::DocViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

DocView* DocViewCreator::CreateView(const IDoc* pDoc)
{
	DocView* pView = new DocView(pDoc->GetId());
	RefreshView(pDoc,pView);
	return pView;
}

void DocViewCreator::RefreshView(const IDoc* pDoc,DocView* pView)
{
	pView->SetDocNo(DocNoText(pDoc).ToString());
	pView->SetDocDate(pDoc->GetDocDate());
	pView->SetLoadDate(pDoc->GetLoadDate());
	pView->SetExtras(pDoc->GetExtras());
	pView->SetPlateNo(pDoc->GetPlateNo());
	pView->SetMotive(pDoc->GetMotive());	
	pView->SetLoadStartDtTm(pDoc->GetLoadStartDtTm());
	pView->SetLoadEndDtTm(pDoc->GetLoadEndDtTm());	

	uint32_t agentId = pDoc->GetAgentId();
	if(agentId != NULL_ID)
	{
		IAgent* pAgent = m_pFacade->GetAgent(agentId);
		pView->SetAgentCd(SafeGet(&IAgent::GetAgentCode,pAgent,L""));
	}	
	
	pView->SetCowCount(pDoc->GetCowCount());	
}

