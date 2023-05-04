#include "stdafx.h"
#include "CollectorListContent.h"
#include <arrays/DeepDelete.h>
#include "../CollectorStatusColors.h"

CollectorListRow::CollectorListRow(const ICollector* collector)
{
	m_Content = new Collector(collector);
}

CollectorListRow::~CollectorListRow()
{
	delete m_Content;
}

std::wstring CollectorListRow::CellValue(uint32_t colId)
{
	return m_CollectorText.Text(m_Content, colId);
}

Color CollectorListRow::CellColor(uint32_t colId)
{
	Color result;
	if (colId == CollectorMember::Status)
	{
		switch (m_Content->GetStatus())
		{
		case CollectorDeviceStatus_Busy:
			result = BUSY_COLOR;
			break;
		case CollectorDeviceStatus_Idle:
			result = IDLE_COLOR;
			break;
		case CollectorDeviceStatus_Syncing:
			result = SYNCING_COLOR;
			break;
		case CollectorDeviceStatus_ReadyToSync:
			result = READY_COLOR;
			break;

		}
	}
	return result;
}

Collector* CollectorListRow::Content() const
{
	return m_Content;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CollectorListContent::CollectorListContent()
{
	
}

CollectorListContent::~CollectorListContent()
{
	DestroyContent();
}

void CollectorListContent::AddCollector(const ICollector* collector)
{
	m_Content.push_back(new CollectorListRow(collector));
	NotifyCountChanged();
}


CollectorListRow* CollectorListContent::FindRow(uint32_t id)
{
	RowsIndex::index<tag_collectorId>::type& idindex = m_Content.get<tag_collectorId>();

	auto it = idindex.find(id);
	if (it != idindex.end())
	{
		return *it;
	}
	return NULL;
}

void CollectorListContent::UpdateCollector(const ICollector* collector)
{
	CollectorListRow* row = FindRow(collector->GetId());

	if (row != NULL)
	{
		row->Content()->CopyFrom(collector);
		NotifyContentUpdated();
	}
	
}

void CollectorListContent::RemoveCollector(uint32_t id)
{
	RowsIndex::index<tag_collectorId>::type& idindex = m_Content.get<tag_collectorId>();

	auto it = idindex.find(id);
	if (it != idindex.end())
	{
		delete *it;
		idindex.erase(it);
		NotifyCountChanged();
	}
	
}

void CollectorListContent::DestroyContent()
{
	DeepDelete(m_Content);
	m_Content.clear();
}
	
int CollectorListContent::GetRowCount() const
{
	return m_Content.size();
}

ListRow* CollectorListContent::GetRow(int row)
{
	return m_Content[row];
}

void CollectorListContent::SortContent(const SortAttributes& attributes)
{
	
}

ICollector* CollectorListContent::GetCollectorAt(int iIndex)
{
	return m_Content[iIndex]->Content();
}

