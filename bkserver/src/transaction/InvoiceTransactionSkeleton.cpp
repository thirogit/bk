#include "stdafx.h"
#include "InvoiceTransactionSkeleton.h"
#include "TransactionException.h"
#include <arrays/DeepDelete.h>
#include <string/TextUtils.h>

InvoiceTransactionSkeleton::InvoiceTransactionSkeleton(ISeasonInterface* pInterface,uint32_t transactionId) : 
	m_transactionId(transactionId),
	m_pInterface(pInterface),
	m_invoiceId(NULL_ID),
	m_invoiceNo(NULL_ID),
	m_type(InvoiceType_None)
{
}

InvoiceTransactionCowItem* InvoiceTransactionSkeleton::FindItem(const ItemId& itemId)
{
	boost::multi_index::index<InvoiceTransactionItemsIndex, tag_itemId>::type& indexByItemId = boost::multi_index::get<tag_itemId>(m_Items);
	auto it = indexByItemId.find(itemId);

	if(indexByItemId.end() != it)
		return *it;

	return NULL;
}

InvoiceTransactionCowItem* InvoiceTransactionSkeleton::AcquireItem(const ItemId& itemId)
{
	InvoiceTransactionCowItem* item = FindItem(itemId);

	if(!item)
		BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"item with id %s not found",ItemIdText::ToString(itemId).c_str())));

	return item;
}

void InvoiceTransactionSkeleton::DeleteDocItems(uint32_t docId)
{
	boost::multi_index::index<InvoiceTransactionItemsIndex, tag_docId>::type& indexByDoc = boost::multi_index::get<tag_docId>(m_Items);
	auto itemRangePair = indexByDoc.equal_range(docId);
		
	auto it = itemRangePair.first;
	while(it != itemRangePair.second)
	{
		delete *it;
		it++;
	}
	indexByDoc.erase(itemRangePair.first,itemRangePair.second);
}

void InvoiceTransactionSkeleton::DeleteItem(const ItemId& itemId)
{
	boost::multi_index::index<InvoiceTransactionItemsIndex, tag_itemId>::type& indexByItemId = boost::multi_index::get<tag_itemId>(m_Items);
	auto it = indexByItemId.find(itemId);

	if(indexByItemId.end() != it)
	{
		InvoiceTransactionCowItem* item = *it;
		indexByItemId.erase(it);
		delete item;
	}
}

ItemId InvoiceTransactionSkeleton::ObtainNewItemId()
{
	ItemId id = m_ItemIdGenerator();
	while(FindItem(id) != NULL)
	{
		id = m_ItemIdGenerator();
	}
	return id;
}

uint32_t InvoiceTransactionSkeleton::GetTransactionId() const
{
	return m_transactionId;
}


void InvoiceTransactionSkeleton::Cleanup()
{
	DeepDelete(m_Items);
	m_Items.clear();
}


void InvoiceTransactionSkeleton::SetHent(const InvoiceHent& hent)
{
	Do([this,&hent](SeasonSessionFacade* facade)
	{
		facade->FindHent(hent.GetParentHentId());
		m_hent.CopyFrom(&hent);
	});
}

DeductionTransactionItem* InvoiceTransactionSkeleton::FindDeduction(const ItemId& itemId)
{
	return m_Deductions.find_item(itemId);
}

ItemId InvoiceTransactionSkeleton::AddDeduction(const IInvoiceDeduction* deduction)
{
	ItemId id = m_ItemIdGenerator();
	DeductionTransactionItem* item = new DeductionTransactionItem(id);
	item->SetDeduction(deduction);
	m_Deductions.add_item(item);
	return id;
}

ItemId InvoiceTransactionSkeleton::AddDeduction(const InvoiceDeduction& deduction)
{
	return AddDeduction(&deduction);
}

void InvoiceTransactionSkeleton::UpdateDeduction(const ItemId& itemId, const InvoiceDeduction& deduction)
{
	DeductionTransactionItem* item = m_Deductions.find_item(itemId);
	if (item != NULL)
	{
		item->SetDeduction(&deduction);
	}
}

void InvoiceTransactionSkeleton::DeleteDeduction(const ItemId& itemId)
{
	DeductionTransactionItem* item = m_Deductions.find_item(itemId);
	if (item != NULL)
	{
		m_Deductions.remove_item(itemId);
		delete item;
	}
}