#include "stdafx.h"
#include "DocItem.h"
#include <data\IIdable.h>

DocItem::DocItem(const ItemId& itemId) : m_itemId(itemId),m_cowId(NULL_ID)
{
}


ItemId DocItem::GetItemId() const
{
	return m_itemId;
}

void DocItem::SetCowDetails(const CowDetails& details)
{
	m_cowDetails.CopyFrom(details);
}

void DocItem::SetCowEntry(const CowEntryDetails& entry)
{
	m_cowEntry.CopyFrom(entry);
}

const CowDetails& DocItem::GetCowDetails() const
{
	return m_cowDetails;
}

const CowEntryDetails& DocItem::GetCowEntry() const
{
	return m_cowEntry;
}

void DocItem::SetCowId(uint32_t cowId)
{
	m_cowId = cowId;
}

uint32_t DocItem::GetCowId() const
{
	return m_cowId;
}