#include "stdafx.h"
#include "HentsDocViewCreator.h"
#include <utils/SafeGet.h>

HentsDocViewCreator::HentsDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade)
{
}

HentsDocView* HentsDocViewCreator::CreateView(IHentsDoc* pDoc)
{
	HentsDocView* pView = new HentsDocView(pDoc->GetId());
	RefreshView(pView,pDoc);	
	return pView;

}

void HentsDocViewCreator::RefreshView(HentsDocView* pView,IHentsDoc* pDoc)
{
	m_docViewCreator.RefreshView(pDoc,pView);
	IHerd* pHerd = m_pFacade->GetHerd(pDoc->GetHerdId());
	pView->SetHerdCd(pHerd->GetHerdName());
	
	HentClientObject* pHentCO = m_pFacade->GetHent(pDoc->GetHentId());
	IHent* pHent = pHentCO->Object();	
	pView->SetHentAlias(pHent->GetAlias());
}

