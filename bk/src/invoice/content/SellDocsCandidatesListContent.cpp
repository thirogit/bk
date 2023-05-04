#include "stdafx.h"
#include "SellDocsCandidatesListContent.h"
#include <utils\SafeGet.h>
#include "../../view/HentsDocViewCreator.h"

void SellDocsCandidatesListContent::CreateContent()
{
	
	PtrEnumerator<SellDocClientObject> sellDocEnum = m_pSession->EnumSellDocs();

	
	HentsDocViewCreator viewCreator(m_pSession);
	HentsDocView* pView;
	SelectableHentsDocRow* pRow;

	while(sellDocEnum.hasNext())
	{
		IHentsDoc* pDoc = (*sellDocEnum)->Object();
		if (pDoc->GetInvoiceId() == NULL_ID)
		{
			pView = viewCreator.CreateView(pDoc);
			pRow = new SelectableHentsDocRow(pView);
			m_Rows.add_row(pRow);
		}
		sellDocEnum.advance();
	}

}

IHentsDoc* SellDocsCandidatesListContent::GetHentsDoc(uint32_t docId)
{
	return SafeGet(&SellDocClientObject::Object,m_pSession->GetSellDoc(docId),nullptr);
}

