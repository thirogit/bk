#include "stdafx.h"
#include "RecognitionSessionContent.h"
#include <arrays/DeepDelete.h>
#include "../view/RecognitionItemViewCreator.h"

RecognitionSessionContent::RecognitionSessionContent(SeasonSession* pSeasonSession) : m_pRecognitionSession(NULL), m_pSeasonSession(pSeasonSession)
{
}

RecognitionSessionContent::~RecognitionSessionContent()
{
}

void RecognitionSessionContent::Create(PassportRecognitionSession* pSession)
{
	m_pRecognitionSession = pSession;
	

	RecognitionItemView* view = NULL;
	IRecognitionItem* item = NULL;
	RecognitionItemViewCreator viewCreator(m_pSeasonSession);
	for (int i = 0, count = m_pRecognitionSession->GetItemCount(); i < count; i++)
	{
		item = m_pRecognitionSession->GetItemAt(i);
		view = viewCreator.CreateView(item);
		AddRow(new RecognitionItemRow(view));
	}

	m_pRecognitionSession->AddObserver(this);
}

void RecognitionSessionContent::AddRow(RecognitionItemRow* row)
{
	m_Rows.push_back(row);
}

int RecognitionSessionContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* RecognitionSessionContent::GetRow(int row)
{
	return m_Rows[row];
}

void RecognitionSessionContent::Destroy()
{
	m_pRecognitionSession = NULL;
	DeepDelete(m_Rows);
}

RecognitionItemRow* RecognitionSessionContent::GetItemAtRow(int row)
{
	return m_Rows[row];
}

void RecognitionSessionContent::RefreshRow(ItemId id)
{
	boost::multi_index::index<IndexType, tag_Key>::type&  index = m_Rows.get<tag_Key>();
	auto rowIt = index.find(id);

	if (rowIt != index.end())
	{
		RecognitionItemRow* pRow = *rowIt;

		IRecognitionItem* pItem = m_pRecognitionSession->GetItem(id);
		RecognitionItemViewCreator viewCreator(m_pSeasonSession);
		RecognitionItemView* pView = pRow->Content();		
		viewCreator.RefreshView(pView, pItem);
		NotifyContentUpdated();
	}
}


void RecognitionSessionContent::OnItemUpdated(ItemId id)
{
	RemoveRow(id);
}

void RecognitionSessionContent::OnItemAdded(ItemId id)
{
	IRecognitionItem* pItem = m_pRecognitionSession->GetItem(id);
	RecognitionItemViewCreator viewCreator(m_pSeasonSession);
	RecognitionItemView* pView = viewCreator.CreateView(pItem);
	AddRow(new RecognitionItemRow(pView));
	NotifyCountChanged();
}

void RecognitionSessionContent::RemoveRow(ItemId id)
{
	boost::multi_index::index<IndexType, tag_Key>::type&  index = m_Rows.get<tag_Key>();
	auto rowIt = index.find(id);

	if (rowIt != index.end())
	{
		index.erase(rowIt);
		NotifyCountChanged();
	}

}

void RecognitionSessionContent::SortContent(const SortAttributes& attributes)
{
	//RowComparator< DataComparator<CowPresenceViewComparator> > comparator(attributes);
	//m_Rows.sort(comparator);
}