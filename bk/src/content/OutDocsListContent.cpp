#include "stdafx.h"
#include "OutDocsListContent.h"
#include "../view/OwnDocViewCreator.h"
#include "../uiconstants/ColorConstants.h"


void OutDocsListContent::CreateContent()
{	
	OwnDocViewCreator viewCreator(m_pSession);
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<IOutDoc> outdocEnum = pRegistry->EnumOutDocs();
	while(outdocEnum.hasNext())
	{
		AddRow(new OwnDocRow(viewCreator.CreateView(*outdocEnum)));
		outdocEnum.advance();
	}		
}

void OutDocsListContent::OnHerdOutDocInserted(uint32_t docId)
{
	OutDocClientObject* pDocCO = m_pSession->GetOutDoc(docId);
	OutDoc* pDoc = pDocCO->Object();
	OwnDocViewCreator viewCreator(m_pSession);
	AddRow(new OwnDocRow(viewCreator.CreateView(pDoc)));
	NotifyCountChanged();
}


void OutDocsListContent::OnHerdOutDocDeleted(uint32_t docId)
{
	OwnDocRow* pRemovedRow = m_Rows.remove_row(docId);
	delete pRemovedRow;
	NotifyCountChanged();
}

void OutDocsListContent::OnHerdOutDocLocked(uint32_t docId)
{
	SetLocked(true,docId);
}

void OutDocsListContent::OnHerdOutDocUnlocked(uint32_t docId)
{
	SetLocked(false,docId);
}

void OutDocsListContent::OnHerdOutDocUpdated(uint32_t docId)
{
	OwnDocRow* pRow = m_Rows.find_row(docId);
	
	OwnDocView* pView = pRow->Content();
	OutDocClientObject* pDocCO = m_pSession->GetOutDoc(pView->GetDocId());
	OutDoc* pDoc = pDocCO->Object();						
	OwnDocViewCreator viewCreator(m_pSession);
	viewCreator.RefreshView(pView,pDoc);
				
	NotifyContentUpdated();
}

void OutDocsListContent::SetLocked(bool bLocked,uint32_t docId)
{
	OwnDocRow* pRow = m_Rows.find_row(docId);
	if(pRow)
		pRow->SetRowColor(bLocked ? Color(LOCKED_ROW_COLOR) : Color());
	NotifyContentUpdated();
}
