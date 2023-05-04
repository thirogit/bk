#include "stdafx.h"
#include "FatOwnDocViewCreator.h"
#include "CowPresenceView.h"

FatOwnDocViewCreator::FatOwnDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade),m_presenceViewCreator(facade)
{
}

FatOwnDocView* FatOwnDocViewCreator::CreateView(const IOwnDoc* pDoc)
{
	FatOwnDocView* pView = new FatOwnDocView(pDoc->GetId());
	RefreshView(pView,pDoc);	
	return pView;
}

void FatOwnDocViewCreator::RefreshView(FatOwnDocView*pView,const IOwnDoc* pDoc)
{
	m_docViewCreator.RefreshView(pView,pDoc);

	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	ICowEntry* entry;
	CowPresenceView* pPresenceView;
	HerdRegistry* pRegistry = m_pFacade->GetHerdRegistry(pDoc->GetHerdId());
	uint32_t cowId;

	while(cowsEnum.hasNext())
	{
		entry = *cowsEnum;
		cowId = entry->GetCowId();
		CowPresenceKey key(cowId,pDoc->GetDocType(),pDoc->GetId());
		pPresenceView = pView->AddCow(key);
		m_presenceViewCreator.RefreshView(pPresenceView,pRegistry->GetCowPresence(key));
		cowsEnum.advance();
	}


}
