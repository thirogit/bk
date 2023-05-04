#include "stdafx.h"
#include "ListHeaderDescriptor.h"
#include <arrays/DeepDelete.h>

ListHeaderDescriptor::ListHeaderDescriptor()
{
}

ListHeaderDescriptor::~ListHeaderDescriptor()
{	
	DeepDelete(m_Columns);
}

ListHeaderDescriptor::ListHeaderDescriptor(const ListHeaderDescriptor& src)
{
	operator=(src);	
}

void ListHeaderDescriptor::AddColumn(const HeaderColumn& column)
{
	m_Columns.push_back(new HeaderColumn(column));
}

ListHeaderDescriptor& ListHeaderDescriptor::operator()(uint32_t colId,const std::wstring& colText)
{
	m_Columns.push_back(new HeaderColumn(colId,colText));
	return *this;
}

PtrEnumerator<HeaderColumn> ListHeaderDescriptor::EnumColumns() const
{
	return PtrEnumerator<HeaderColumn>(m_Columns);
}

ListHeaderDescriptor& ListHeaderDescriptor::operator=(const ListHeaderDescriptor& right)
{
	DeepDelete(m_Columns);
	m_Columns.clear();
	
	auto headerColIt = right.m_Columns.begin();
	HeaderColumn* pCol;
	while(headerColIt !=  right.m_Columns.end())
	{
		pCol = new HeaderColumn(**headerColIt);
		m_Columns.push_back(pCol);	
		headerColIt++;
	}
	return *this;
}

const HeaderColumn* ListHeaderDescriptor::GetColumnWithId(uint32_t colId) const
{
	const ColumnIndex::nth_index<1>::type& index = m_Columns.get<1>();

	auto colIt = index.find(colId);
	if(colIt != index.end())
		return *colIt;
	return NULL;
}
