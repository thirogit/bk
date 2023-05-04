#include "stdafx.h"
#include "MoveDocViewCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


MoveDocViewCreator::MoveDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade)
{
}

MoveDocView* MoveDocViewCreator::CreateView(const IMoveDoc* pDoc)
{
	MoveDocView* pView = new MoveDocView(pDoc->GetId());
	RefreshView(pView,pDoc);
	return pView;
}

void MoveDocViewCreator::RefreshView(MoveDocView* pView,const IMoveDoc* pDoc)
{
	m_docViewCreator.RefreshView(pDoc,pView);
	
	IHerd* pSrcHerd = m_pFacade->GetHerd(pDoc->GetSrcHerdId());
	pView->SetSrcHerdCd(pSrcHerd->GetHerdName());
	
	IHerd* pDstHerd = m_pFacade->GetHerd(pDoc->GetDstHerdId());
	pView->SetDstHerdCd(pDstHerd->GetHerdName());
}


