#include "stdafx.h"
#include "BuyInvoiceCowsSelectInterface.h"


UniqueIds BuyInvoiceCowsSelectInterface::GetCowsSelection()
{
	UniqueIds selection;
	uint32_t docId = m_transaction->GetDocId();
	InDocClientObject* pInDocCO = m_pSession->GetInDoc(docId);
	InDoc* pInDoc = pInDocCO->Object();

	PtrEnumerator<ICowEntry> cowsEnum = pInDoc->EnumCows();

	ICowEntry* pEntry = NULL;
	CowClientObject* pCowCO;
	uint32_t cowId;
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();
		pCowCO = m_pSession->GetCow(cowId);

		if(pCowCO->Object()->GetBuyInvoiceId() ==  NULL_ID && m_transaction->GetItem(cowId) == NULL)
		{		
			selection.add(cowId);
		}
		cowsEnum.advance();
	}
	return selection;
}

