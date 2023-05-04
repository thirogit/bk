#include "stdafx.h"
#include "AllCowsListContent.h"
#include "../view/CowPresenceViewCreator.h"
#include "CowPresenceRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void AllCowsListContent::CreateContent()
{
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<ICowPresence> cowsEnum = pRegistry->EnumCows();
	CowPresenceViewCreator viewCreator(m_pSession);
	while(cowsEnum.hasNext())
	{
		ICowPresence* pPresence = *cowsEnum;
		//TRACE("cowId = %d, herd = %d\n",pPresence->GetCowId(),GetHerdId());
		//if(pPresence->GetCowId() == 28670)
		//	TRACE("got it\n");
		CowPresenceView* pCowView = viewCreator.CreateView(pPresence);
		AddRow(new CowPresenceRow(pCowView));
		cowsEnum.advance();										
	}
	//TRACE("end CreateContent()\n");
}

//void AllCowsListContent::OnHerdCowMoved(const CowPresenceKey& key)
//{
//	RefreshCowRows(key.GetCowId());
//}