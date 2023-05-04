#include "stdafx.h"
#include "SellInvoiceCowsSelectInterface.h"


UniqueIds SellInvoiceCowsSelectInterface::GetCowsSelection()
{
	UniqueIds selection;
	uint32_t docId = m_transaction->GetDocId();
	OutDocClientObject* pDocCO = m_pSession->GetOutDoc(docId);
	OutDoc* pDoc = pDocCO->Object();

	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();

	ICowEntry* pEntry = NULL;
	CowClientObject* pCowCO;
	uint32_t cowId;
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();
		pCowCO = m_pSession->GetCow(cowId);

		if(pCowCO->Object()->GetSellInvoiceId() ==  NULL_ID && m_transaction->GetItem(cowId) == NULL)
		{		
			selection.add(cowId);
		}
		cowsEnum.advance();
	}
	return selection;
}

