#include "stdafx.h"
#include "InCowsListContent.h"
#include "../view/CowPresenceViewCreator.h"
#include "CowPresenceRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void InCowsListContent::CreateContent()
{
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<ICowPresence> cowsEnum = pRegistry->EnumCows();
	CowPresenceViewCreator viewCreator(m_pSession);
	while(cowsEnum.hasNext())
	{
		ICowPresence* pPresence = *cowsEnum;
		if(pPresence->IsInStock())
		{			
			CowPresenceView* pCowView = viewCreator.CreateView(pPresence);
			AddRow(new CowPresenceRow(pCowView));
		}
		cowsEnum.advance();										
	}
};

void InCowsListContent::OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey)
{
	RemoveRow(fromKey);
}

void  InCowsListContent::OnHerdCowRemoved(const CowPresenceKey& key) 
{
	RemoveRow(key);
}

void InCowsListContent::OnHerdCowRevoked(const CowPresenceKey& fromKey,const CowPresenceKey& toKey)
{	
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	CowPresenceViewCreator viewCreator(m_pSession);
	ICowPresence* pPresence = pRegistry->GetCowPresence(toKey);
	CowPresenceView* pCowView = viewCreator.CreateView(pPresence);
	AddRow(new CowPresenceRow(pCowView));	
	NotifyCountChanged();
}