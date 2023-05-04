#include "stdafx.h"
#include "InDocsListContent.h"
#include "../view/OwnDocViewCreator.h"
#include "../context/object/InDocClientObject.h"
#include "../uiconstants/ColorConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void InDocsListContent::CreateContent()
{	
	OwnDocViewCreator viewCreator(m_pSession);
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<IInDoc> indocEnum = pRegistry->EnumInDocs();
	while(indocEnum.hasNext())
	{
		IInDoc* pInDoc = *indocEnum;
		InDocClientObject* pInDocCO = m_pSession->GetInDoc(pInDoc->GetId());
		OwnDocRow* pRow = new OwnDocRow(viewCreator.CreateView(pInDoc));
		pRow->SetRowColor(pInDocCO->IsLocked() ? Color(LOCKED_ROW_COLOR) : Color());
		AddRow(pRow);
		indocEnum.advance();
	}
}

void InDocsListContent::OnHerdInDocInserted(uint32_t docId)
{
	InDocClientObject* pInDocCO = m_pSession->GetInDoc(docId);
	InDoc* pInDoc = pInDocCO->Object();						
	OwnDocViewCreator viewCreator(m_pSession);
	AddRow( new OwnDocRow(viewCreator.CreateView(pInDoc)));						

	NotifyCountChanged();
}

void InDocsListContent::OnHerdInDocUpdated(uint32_t docId)
{
	OwnDocRow* pRow = m_Rows.find_row(docId);

	OwnDocView* pView = pRow->Content();
	InDocClientObject* pInDocCO = m_pSession->GetInDoc(pView->GetDocId());
	InDoc* pInDoc = pInDocCO->Object();						
	OwnDocViewCreator viewCreator(m_pSession);
	viewCreator.RefreshView(pView,pInDoc);

	NotifyContentUpdated();
}

void InDocsListContent::SetLocked(bool bLocked,uint32_t docId)
{
	OwnDocRow* pRow = m_Rows.find_row(docId);
	if(pRow)
		pRow->SetRowColor(bLocked ? Color(LOCKED_ROW_COLOR) : Color());
	NotifyContentUpdated();
}

void InDocsListContent::OnHerdInDocLocked(uint32_t docId)
{
	SetLocked(true,docId);
}

void InDocsListContent::OnHerdInDocUnlocked(uint32_t docId)
{
	SetLocked(false,docId);
}

void InDocsListContent::OnHerdInDocDeleted(uint32_t docId)
{
	OwnDocRow* pRemovedRow = m_Rows.remove_row(docId);
	delete pRemovedRow;
	NotifyCountChanged();
}