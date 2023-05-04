#include "stdafx.h"
#include "MoveDocsListContent.h"
#include "../view/MoveDocViewCreator.h"
#include <arrays\DeepDelete.h>

#include "../compare/DataComparator.h"
#include "compare/RowComparator.h"
#include "../compare/MoveDocComparator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MoveDocsListContent::MoveDocsListContent() : m_pSession(NULL),m_herdId(NULL_ID)
{
}

MoveDocsListContent::~MoveDocsListContent()
{	
}

void MoveDocsListContent::Create(SeasonSession* pSession,uint32_t herdId)
{
	m_pSession = pSession;
	m_herdId = herdId;
	
	MoveDocViewCreator viewCreator(m_pSession);
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(m_herdId);
	PtrEnumerator<IMoveDoc> movdocEnum = pRegistry->EnumMoveDocs();
	while(movdocEnum.hasNext())
	{
		AddRow(new MoveDocRow(viewCreator.CreateView(*movdocEnum)));
		movdocEnum.advance();
	}
	m_pSession->AddHerdObserver(m_herdId,this);
		
}

uint32_t MoveDocsListContent::GetHerdId() const
{
	return m_herdId;
}


int MoveDocsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* MoveDocsListContent::GetRow(int row)
{
	return GetMoveDocAtRow(row);
}

MoveDocRow* MoveDocsListContent::GetMoveDocAtRow(int row)
{
	return m_Rows[row];
}

void MoveDocsListContent::Destroy()
{
	if(m_pSession)
		m_pSession->RemoveHerdObserver(m_herdId,this);
	
	m_pSession = NULL;
	m_herdId = NULL_ID;
	DeepDelete(m_Rows);	
}

void MoveDocsListContent::AddRow(MoveDocRow* row)
{
	m_Rows.push_back(row);
}

void MoveDocsListContent::OnHerdMoveDocInserted(uint32_t docId)
{
	MoveDocClientObject* pDocCO = m_pSession->GetMoveDoc(docId);
	MoveDoc* pInDoc = pDocCO->Object();						
	MoveDocViewCreator viewCreator(m_pSession);
	AddRow(new MoveDocRow(viewCreator.CreateView(pInDoc)));						

	NotifyCountChanged();
}

void MoveDocsListContent::RemoveRow(uint32_t docId)
{
	boost::multi_index::index<RowsIndex,tag_docId>::type& index = m_Rows.get<tag_docId>();
	auto rowIt = index.find(docId);	
	if(rowIt != index.end())
	{
		MoveDocRow* pRow = *rowIt;
		index.erase(rowIt);	
		delete pRow;		
	}
}

void MoveDocsListContent::RefreshRowForDoc(uint32_t docId)
{
	boost::multi_index::index<RowsIndex,tag_docId>::type& index = m_Rows.get<tag_docId>();
	auto rowIt = index.find(docId);	
	if(rowIt != index.end())
	{
		MoveDocRow* pRow = *rowIt;
		
		MoveDocClientObject* pDocCO = m_pSession->GetMoveDoc(docId);
		MoveDoc* pDoc = pDocCO->Object();						
		MoveDocViewCreator viewCreator(m_pSession);
		viewCreator.RefreshView(pRow->Content(),pDoc);						
	
	}
}

void MoveDocsListContent::OnHerdMoveDocDeleted(uint32_t docId)
{
	RemoveRow(docId);
	NotifyCountChanged();
}

void MoveDocsListContent::OnHerdMoveDocLocked(uint32_t docId)
{
	RefreshRowForDoc(docId);
	NotifyContentUpdated();
}

void MoveDocsListContent::OnHerdMoveDocUnlocked(uint32_t docId)
{
	RefreshRowForDoc(docId);
	NotifyContentUpdated();
}

void MoveDocsListContent::OnHerdMoveDocUpdated(uint32_t docId)
{
	RefreshRowForDoc(docId);
	NotifyContentUpdated();
}

void MoveDocsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<MoveDocComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}