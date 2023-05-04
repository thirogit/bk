#include "stdafx.h"
#include "DocTransactionSkeleton.h"
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <types/DateRange.h>
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include <errortoken/ErrorToken.h>

DocTransactionSkeleton::DocTransactionSkeleton(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : 
	m_pInterface(pInterface), m_TransactionId(transactionId), LoggerAware(logger)
{
}

DocTransactionCowItem* DocTransactionSkeleton::GetItemAt(int index)
{
	return m_Items.item_at(index);
}

DocTransactionSkeleton::~DocTransactionSkeleton()
{
	DeepDelete(m_Items);
}

uint32_t DocTransactionSkeleton::GetTransactionId() const
{
	return m_TransactionId;
}

void DocTransactionSkeleton::SetDocDetails(const DocDetails& details)
{	
	m_docDetails.CopyFrom(details);
}

ItemId DocTransactionSkeleton::ObtainNewItemId()
{
	ItemId id = m_ItemIdGenerator();
	while(m_Items.find_item(id) != NULL)
	{
		id = m_ItemIdGenerator();
	}
	return id;
}

ItemId DocTransactionSkeleton::AddCow(uint32_t cowId,const CowDetails& details)
{	
	ItemId id = ObtainNewItemId();
	DocTransactionCowItem* pNewItem = new DocTransactionCowItem(id,cowId);
	pNewItem->SetCow(details);
	m_Items.add_item(pNewItem);
	return id;
}

DocTransactionCowItem* DocTransactionSkeleton::FindItem(ItemId itemId)
{
	DocTransactionCowItem* pItem = m_Items.find_item(itemId);
	if(!pItem)
	{
		ThrowItemNotFound(itemId);
	}
	
	return pItem;
}

void DocTransactionSkeleton::ThrowItemNotFound(ItemId itemId)
{
	BOOST_THROW_EXCEPTION(TransactionException(TextUtils::Format(L"item with id = %s does not belong to transaction", ItemIdText::ToString(itemId))));
}

void DocTransactionSkeleton::DeleteCow(ItemId itemId)
{	
	DocTransactionCowItem* pItem = m_Items.find_item(itemId);
	if(!pItem)
	{
		ThrowItemNotFound(itemId);	
	}	
	m_Items.remove_item(itemId);	
	delete pItem;
}


void DocTransactionSkeleton::UpdateEntry(ItemId itemId, const CowEntryDetails& details)
{
	DocTransactionCowItem* item = FindItem(itemId);
	item->SetEntry(details);
}

count_t DocTransactionSkeleton::GetItemsCount() const
{
	return m_Items.size();
}

void DocTransactionSkeleton::Clear()
{
	DeepDelete(m_Items);
	m_Items.clear();
}

DocTransactionCowItem*  DocTransactionSkeleton::FindItem(std::function<bool (DocTransactionCowItem*)>& predicate)
{
	BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
	{
		if(predicate(item))
			return item;
	}

	return NULL;
}
