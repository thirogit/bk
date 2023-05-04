#include "stdafx.h"
#include "ImportItem.h"

ImportItem::ImportItem(ItemId id) : m_id(id)
{
}

ItemId ImportItem::GetId() const
{
	return m_id;
}

ImportProblemRange ImportItem::Problems()
{
	return ImportProblemRange(m_problems);
}

ImportProblem* ImportItem::GetProblem(ItemId id)
{
	return m_problems.find_item(id);
}



