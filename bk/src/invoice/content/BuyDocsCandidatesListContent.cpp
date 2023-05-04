#include "stdafx.h"
#include "BuyDocsCandidatesListContent.h"
#include <utils\SafeGet.h>
#include "../../view/HentsDocViewCreator.h"

void BuyDocsCandidatesListContent::CreateContent()
{
	PtrEnumerator<BuyDocClientObject> buyDocEnum = m_pSession->EnumBuyDocs();


	HentsDocViewCreator viewCreator(m_pSession);
	HentsDocView* pView;
	SelectableHentsDocRow* pRow;

	while (buyDocEnum.hasNext())
	{
		IHentsDoc* pDoc = (*buyDocEnum)->Object();
		if (pDoc->GetInvoiceId() == NULL_ID)
		{
			pView = viewCreator.CreateView(pDoc);
			pRow = new SelectableHentsDocRow(pView);
			m_Rows.add_row(pRow);
		}
		buyDocEnum.advance();
	}
}

IHentsDoc* BuyDocsCandidatesListContent::GetHentsDoc(uint32_t docId)
{
	return SafeGet(&BuyDocClientObject::Object,m_pSession->GetBuyDoc(docId),nullptr);
}
	