#include "stdafx.h"
#include "InvoiceItemIndex.h"
#include <boost\foreach.hpp>

InvoiceTransactionItem* InvoiceItemIndex::find_item(ItemId id)
{
	boost::multi_index::index<Index,tag_itemId>::type& indexByItemKey = boost::multi_index::get<tag_itemId>(m_items);

	auto itemIt = indexByItemKey.find(id);
	if(itemIt != indexByItemKey.end())
		return *itemIt;

	return NULL;
}

void InvoiceItemIndex::push_back(InvoiceTransactionItem* item)
{
	m_items.push_back(item);
}

size_t InvoiceItemIndex::size() const
{
	return m_items.size();
}


InvoiceTransactionItem* InvoiceItemIndex::get_at(int index)
{
	return m_items[index];
}

InvoiceTransactionItem* InvoiceItemIndex::find_item(uint32_t cowId)
{
	boost::multi_index::index<Index,tag_cowId>::type& indexByCowId = boost::multi_index::get<tag_cowId>(m_items);

	auto itemIt = indexByCowId.find(cowId);
	if(indexByCowId.end() != itemIt)
		return *itemIt;

	return NULL;
}

std::vector<InvoiceTransactionItem*> InvoiceItemIndex::delete_items(uint32_t docId)
{
	boost::multi_index::index<Index,tag_docId>::type& indexByDocId = boost::multi_index::get<tag_docId>(m_items);

	auto itemsRange = indexByDocId.equal_range(docId);

	std::vector<InvoiceTransactionItem*> deletedItems;
	BOOST_FOREACH(InvoiceTransactionItem* item, itemsRange)
	{
		deletedItems.push_back(item);
	}
	indexByDocId.erase(itemsRange.first,itemsRange.second);

	return std::move(deletedItems);
}

InvoiceItemIndex::const_iterator InvoiceItemIndex::begin() const
{
	return m_items.cbegin();
}

InvoiceItemIndex::const_iterator InvoiceItemIndex::end() const
{
	return m_items.cend();
}


InvoiceItemIndex::iterator InvoiceItemIndex::begin()
{
	return m_items.begin();
}

InvoiceItemIndex::iterator InvoiceItemIndex::end()
{
	return m_items.end();
}

InvoiceTransactionItem* InvoiceItemIndex::delete_item(ItemId id)
{
	boost::multi_index::index<Index,tag_itemId>::type& indexByItemId = boost::multi_index::get<tag_itemId>(m_items);

	auto itemIt = indexByItemId.find(id);
	if(itemIt != indexByItemId.end())
	{
		InvoiceTransactionItem* removed_item = *itemIt;
		indexByItemId.erase(itemIt);
		return removed_item;
	}
	return NULL;
}