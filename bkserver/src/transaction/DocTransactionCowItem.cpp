#include "stdafx.h"
#include "DocTransactionCowItem.h"

//-------------------------------------------------------------------------------------------------

DocTransactionCowItem::DocTransactionCowItem(ItemId itemId,uint32_t cowId) : m_ItemId(itemId),DocCowItem(cowId)
{
}

ItemId DocTransactionCowItem::GetId() const
{
	return m_ItemId;
}
