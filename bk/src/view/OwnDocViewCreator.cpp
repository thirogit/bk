#include "stdafx.h"
#include "OwnDocViewCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


OwnDocViewCreator::OwnDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade)
{
}

OwnDocView* OwnDocViewCreator::CreateView(const IOwnDoc* pDoc)
{
	OwnDocView* pView = new OwnDocView(pDoc->GetId());
	RefreshView(pView,pDoc);	
	return pView;
}

void OwnDocViewCreator::RefreshView(OwnDocView*pView,const IOwnDoc* pDoc)
{
	m_docViewCreator.RefreshView(pDoc,pView);
	IHerd* pHerd = m_pFacade->GetHerd(pDoc->GetHerdId());
	pView->SetHerdCd(pHerd->GetHerdName());
}
