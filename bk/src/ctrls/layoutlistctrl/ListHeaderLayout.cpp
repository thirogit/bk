#include "stdafx.h"
#include "ListHeaderLayout.h"

ListHeaderLayout::ListHeaderLayout()
{
}

ListHeaderLayout::~ListHeaderLayout()
{
	Clear();
}

void ListHeaderLayout::Clear()
{
	m_ColumnLayouts.FreeAndFlush();
}

ListHeaderLayout::ListHeaderLayout(const ListHeaderLayout& layout)
{
	operator=(layout);
}

ListHeaderLayout& ListHeaderLayout::operator=(const ListHeaderLayout& right)
{
	m_ColumnLayouts.FreeAndFlush();
	auto colIt = right.m_ColumnLayouts.begin();
	while(colIt != right.m_ColumnLayouts.end())
	{
		m_ColumnLayouts.Add(new ColumnLayout(**colIt));
	}

	return *this;
}

void ListHeaderLayout::AddColumnLayout(uint32_t colId, int colWidth)
{
	m_ColumnLayouts.Add(new ColumnLayout(colId,colWidth));
}

void ListHeaderLayout::RemoveColumnLayout(uint32_t colId)
{
	auto colIt = std::find_if(m_ColumnLayouts.begin(),m_ColumnLayouts.end(),[colId](ColumnLayout* pColumnLayout) -> bool
																			{
																				return pColumnLayout->GetColId() == colId;
																			});

	if (colIt != m_ColumnLayouts.end())
	{
		delete *colIt;
		m_ColumnLayouts.erase(colIt);
	}
}

PtrEnumerator<ColumnLayout> ListHeaderLayout::EnumColumns()
{
	return PtrEnumerator<ColumnLayout>(m_ColumnLayouts);
}

bool ListHeaderLayout::IsEmpty() const
{
	return m_ColumnLayouts.size() == 0;
}

ColumnLayout* ListHeaderLayout::GetColumnAt(int nCol)
{
	return m_ColumnLayouts.GetAt(nCol);
}

int ListHeaderLayout::GetColumnCount() const
{
	return m_ColumnLayouts.size();
}

bool ListHeaderLayout::ContainsColumnWithId(uint32_t colId)
{
	auto colIt = m_ColumnLayouts.begin();
	while(colIt != m_ColumnLayouts.end())
	{
		if((*colIt)->GetColId() == colId)
			return true;

		colIt++;
	}
	return false;
}

void ListHeaderLayout::MoveColumn(int srcIndex, int dstIndex)
{
	auto it = m_ColumnLayouts.begin() + srcIndex;
	ColumnLayout* column = *it;
	m_ColumnLayouts.erase(it);
	it = m_ColumnLayouts.begin() + dstIndex;
	m_ColumnLayouts.insert(it, column);
}